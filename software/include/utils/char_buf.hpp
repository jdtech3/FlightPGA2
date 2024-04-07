#pragma once

#include <string>

#include "cstdint_short.hpp"
#include "utils/logging.hpp"


class CharBuf {
    private:
        volatile char* char_buf_start_;

    public:
        const static int CHAR_BUF_WIDTH = 80;
        const static int CHAR_BUF_HEIGHT = 60;

        CharBuf(u32 char_buf_address);
        void println(std::string str, u8 line = 0, u8 offset = 0);
        void eraseln(u8 line = 0);
        void clear();
};
