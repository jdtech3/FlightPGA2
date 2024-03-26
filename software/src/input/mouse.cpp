#include "input/mouse.hpp"

// -- Globals

static volatile ps2_reg_t* ps2_reg_ptr_ = reinterpret_cast<ps2_reg_t*>(PS2_PORT_BASE);
static mouse_pos_t cur_pos_;

// -- Utils

static read_result_t read_data_(bool check_timeout = true) {
    u32 data_reg;
    bool rvalid;
    u32 count = 0;
    do {
        data_reg = ps2_reg_ptr_->data;
        rvalid = (data_reg >> 15) & 1;
        count++;
    } while (!(rvalid || (check_timeout && (count > PS2_TIMEOUT))));
    if (check_timeout && count > PS2_TIMEOUT) return { 0, true };

    return { data_reg & 0xFF, false };
}

static void write_data_(u8 data) {
    ps2_reg_ptr_->data = data;
    if (read_data_(false).data != 0xFA)
        std::cout << "PS/2 mouse write: NACK" << std::endl;
}

static void clear_fifo_() {
    u32 data_reg = ps2_reg_ptr_->data;
    while (((data_reg >> 15) & 1) != 0) {
        data_reg = ps2_reg_ptr_->data;
    }
}

static mouse_pos_t read_mouse_pos_change_() {
    std::array<read_result_t, 3> data = {};
    data[0] = read_data_();
    data[1] = read_data_();
    data[2] = read_data_();

    if (std::any_of(data.begin(), data.end(), [](read_result_t r) { return r.timed_out; })) 
        return (mouse_pos_t) { 0.0f, 0.0f };     // if any read timed out, we're done

    // Translate
    bool x_sign = (data[0].data >> 4) & 1;
    bool y_sign = (data[0].data >> 5) & 1;

    return (mouse_pos_t) {
        (x_sign ? -1.0f : 1.0f) * static_cast<float>(data[1].data) / 4.0f,  // 4 counts per mm
        (y_sign ? -1.0f : 1.0f) * static_cast<float>(data[2].data) / 4.0f
    };
}

// -- ISR

static void mouse_isr_(void* context, alt_u32 id) {
    auto cur_pos_ptr = reinterpret_cast<mouse_pos_t*>(context);

    while (true) {  // more intelligently written loop is probably possible
        mouse_pos_t pos_change = read_mouse_pos_change_();
        cur_pos_ptr->x += pos_change.x;
        cur_pos_ptr->y += pos_change.y;
    }
    clear_fifo_();
}

// -- Public

void init_mouse_isr() {
    cur_pos_.x = 0.0f;
    cur_pos_.y = 0.0f;

    // Init PS/2 mouse
    clear_fifo_();
    write_data_(0xFF);          // reset
    u8 data = read_data_(false).data;
    if (data == 0xAA) {
        data = read_data_(false).data;
        if (data == 0x00)
            write_data_(0xF4);  // enable data reporting
        else
            std::cout << "PS/2 mouse init 2/2: didn't get 0x00, got " << static_cast<int>(data) << std::endl;
    }
    else
        std::cout << "PS/2 mouse init 1/2: didn't get 0xAA, got " << static_cast<int>(data) << std::endl;

    // Clear FIFO
    clear_fifo_();
    ps2_reg_ptr_->control = 1;  // enable interrupts

    // Support both enhanced and legacy APIs
    #ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
            alt_ic_isr_register(
            PS2_PORT_IRQ_INTERRUPT_CONTROLLER_ID,
            PS2_PORT_IRQ,
            &mouse_isr_, &cur_pos_, 0
        );
        alt_ic_irq_enable(PS2_PORT_IRQ_INTERRUPT_CONTROLLER_ID, PS2_PORT_IRQ);
    #else
        alt_irq_register(PS2_PORT_IRQ, &cur_pos_, &mouse_isr_);
        alt_irq_enable(PS2_PORT_IRQ);    
    #endif
    alt_irq_cpu_enable_interrupts();
}

float get_mouse_mm_x() { return cur_pos_.x; }
float get_mouse_mm_y() { return cur_pos_.y; };
