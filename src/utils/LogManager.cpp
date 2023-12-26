#include "LogManager.hpp"

namespace cmd {
    char fmt_buffer[FMT_BUFFER_SIZE];

    namespace scope {
        #ifdef APPLY_COLORS
        void printing(const msg_side arg1, const msg_type arg2) {
            // std::chrono::system_clock::now()
            //              vs
            // fmt::localtime(std::time(nullptr))
            // - Este ultimo respeta la la zona horaria de la computadora.
            fmt::print(
                "{} {} {} {}\n",
                fmt::format(fg(fmt::color::slate_gray), "[{:%T}]", fmt::localtime(std::time(nullptr))),
                fmt::format(fg(side::colors[arg1]), "[{}]", side::msg[arg1]),
                fmt::format(fg(type::colors[arg2]), "{}", type::msg[arg2]),
                fmt_buffer
            );
        }
        #else
        void printing(const msg_side arg1, const msg_type arg2) {
            fmt::print(
                "[{:%T}] [{}] {} {}\n",
                fmt::localtime(std::time(nullptr)),
                side::msg[arg1],
                type::msg[arg2],
                fmt_buffer
            );
        }
        #endif
    }
};