#include "utils/uart.hpp"

// -- Globals

static jtag_uart_reg_t* jtag_uart_reg_ptr_ = reinterpret_cast<jtag_uart_reg_t*>(JTAG_2_UART_BASE);

// -- Public

namespace uart {
    bool is_host_connected() {
        return (jtag_uart_reg_ptr_->control >> 10) & 1;
    }

    // Returns 0 if nothing available
    u8 get_char(bool echo) {
        u32 data = jtag_uart_reg_ptr_->data;
        if ((data >> 15) & 1) {
            u8 ch = data & 0xFF;
            if (echo) put_char(ch);
            return ch;
        }
        else
            return 0;
    }

    bool put_char(u8 ch) {
        if (((jtag_uart_reg_ptr_->control >> 16) & 0xFFFF) > 0) {
            jtag_uart_reg_ptr_->data = ch;
            return true;
        }
        else return false;
    }

    void clear_fifo() {
        while (get_char() != 0);
    }
}