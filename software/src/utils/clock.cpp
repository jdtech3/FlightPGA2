#include "utils/clock.hpp"

// -- Globals

static volatile interval_timer_reg_t* interval_timer_reg_ptr_ = reinterpret_cast<interval_timer_reg_t*>(INTERVAL_TIMER_2_BASE);
static u32 sys_time_ms_;

// -- ISR

static void timer_isr_(void* context, alt_u32 id) {
    (*reinterpret_cast<u32*>(context))++;   // add to ms counter
    interval_timer_reg_ptr_->status = 0;    // reset timer
}

// -- Public

void init_timer_isr() {
    u32 counter_start = INTERVAL_TIMER_2_FREQ / 1000;
    interval_timer_reg_ptr_->counter_start_low = counter_start & 0xFFFF;    // set counter to 1ms
    interval_timer_reg_ptr_->counter_start_high = counter_start >> 16;
    interval_timer_reg_ptr_->status = 0;        // reset
    interval_timer_reg_ptr_->control = 0b0111;  // set START, CONT, ITO

    // Support both enhanced and legacy APIs
    #ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
        alt_ic_isr_register(
            INTERVAL_TIMER_2_IRQ_INTERRUPT_CONTROLLER_ID, 
            INTERVAL_TIMER_2_IRQ,
            &timer_isr_, &sys_time_ms_, 0
        );
        alt_ic_irq_enable(INTERVAL_TIMER_2_IRQ_INTERRUPT_CONTROLLER_ID, INTERVAL_TIMER_2_IRQ);
    #else
        alt_irq_register(INTERVAL_TIMER_2_IRQ, &sys_time_ms_, &timer_isr_);
        alt_irq_enable(INTERVAL_TIMER_2_IRQ);
    #endif
    alt_irq_cpu_enable_interrupts();

    std::cout << "Timer ISR init: done" << std::endl;
}

u32 get_clock_ms() { return sys_time_ms_; }
void reset_clock_ms() { sys_time_ms_ = 0; }
