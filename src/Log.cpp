#include "Log.hpp"

namespace cmd {
    char fmt_buffer[FMT_BUFFER_SIZE];

    #ifdef APPLY_COLORS
    void printing(const msg_side arg1, const msg_type arg2) {
        fmt::print(
            "{} {} {} {}\n",
            fmt::format(fg(fmt::color::slate_gray), "[{:%T}]", std::chrono::system_clock::now()),
            fmt::format(fg(side::colors[arg1]), "[{}]", side::msg[arg1]),
            fmt::format(fg(type::colors[arg2]), "{}", type::msg[arg2]),
            fmt_buffer
        );
    }
    #else
    void printing(const msg_side arg1, const msg_type arg2) {
        fmt::print(
            "[{:%T}] [{}] {} {}\n",
            std::chrono::system_clock::now(),
            side::msg[arg1],
            type::msg[arg2],
            fmt_buffer
        );
    }
    #endif
};