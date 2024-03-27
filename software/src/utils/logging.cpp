#include "utils/logging.hpp"

static const std::string CLEAR = "\x1b[0m";
static const std::string RED = "\x1b[31m";
static const std::string GREEN = "\x1b[32m";
static const std::string YELLOW = "\x1b[33m";
static const std::string CYAN = "\x1b[36m";

namespace logging {
    static void log_(log_level_t level, std::string context, std::string msg, bool write_endl) {
        switch (level) {
            case OK:
                std::cout << GREEN << "[ -  ] ";
                break;
            case INFO:
                std::cout << CYAN << "[INFO] ";
                break;
            case WARNING:
                std::cout << YELLOW << "[WARN] ";
                break;
            case ERROR:
                std::cout << RED << "[ERR ] ";
                break;
        }
        std::cout << context << ": " << CLEAR << msg;
        if (write_endl) std::cout << std::endl;
    }

    void ok(std::string context, std::string msg, bool write_endl) { log_(OK, context, msg, write_endl); }
    void info(std::string context, std::string msg, bool write_endl) { log_(INFO, context, msg, write_endl); }
    void warn(std::string context, std::string msg, bool write_endl) { log_(WARNING, context, msg, write_endl); }
    void err(std::string context, std::string msg, bool write_endl) { log_(ERROR, context, msg, write_endl); }
};
