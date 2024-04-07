#include "utils/char_buf.hpp"

CharBuf::CharBuf(u32 char_buf_address) {
    char_buf_start_ = reinterpret_cast<volatile char*>(char_buf_address);
    clear();
}

void CharBuf::println(std::string str, u8 line, u8 offset) {
    if (static_cast<int>(str.size()) > (CHAR_BUF_WIDTH - offset))
        logging::warn("Char buf", "println received string too long: len " + std::to_string(str.size()) + " at offset " + std::to_string(offset) + "; clamping!");

    char* start_addr = reinterpret_cast<char*>(reinterpret_cast<u32>(char_buf_start_) + (line << 7) + offset);
    str.copy(start_addr, CHAR_BUF_WIDTH - offset);
}

void CharBuf::eraseln(u8 line) {
    println(std::string().append(CHAR_BUF_WIDTH, ' '), line);
}

void CharBuf::clear() {
    for (u8 line = 0; line < CHAR_BUF_HEIGHT; line++)
        eraseln(line);
}
