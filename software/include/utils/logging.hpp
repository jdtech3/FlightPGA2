#pragma once

#include <iostream>
#include <string>


typedef enum log_level {
    OK,
    INFO,
    WARNING,
    ERROR
} log_level_t;

namespace logging {
    void ok(std::string context, std::string msg, bool write_endl = true);
    void info(std::string context, std::string msg, bool write_endl = true);
    void warn(std::string context, std::string msg, bool write_endl = true);
    void err(std::string context, std::string msg, bool write_endl = true);
};
