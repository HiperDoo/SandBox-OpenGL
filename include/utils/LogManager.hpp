#pragma once
#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

#define APPLY_COLORS

namespace cmd {
    // NOTE: https://github.com/fmtlib/fmt
    // NOTE: https://hackingcpp.com/cpp/libs/fmt.html#quick-fmt-specs

    namespace side {
        constexpr char msg[3][7] = { "Server", "Client", "OpenGL" };
        #ifdef APPLY_COLORS
        constexpr fmt::rgb colors[3] = {
            {35, 209, 139},
            {85, 85, 255},
            {89, 2, 87}
        };
        #endif
    };
    namespace type {
        constexpr char msg[4][6] = { "INFO ", "WARN ", "ERROR", "DEBUG" };
        #ifdef APPLY_COLORS
        constexpr fmt::rgb colors[4] = {
            {36, 114, 200},
            {255, 215, 68},
            {255, 85, 85},
            {252, 170, 19}
        };
        #endif
    };

    enum msg_side { server = 0, client, opengl };
    enum msg_type { info = 0, warn, error, debug };

    #define FMT_BUFFER_SIZE 128
    extern char fmt_buffer[FMT_BUFFER_SIZE];

    namespace scope {
        #ifdef APPLY_COLORS
        void printing(const msg_side arg1, const msg_type arg2);
        #else
        void printing(const msg_side arg1, const msg_type arg2);
        #endif
    }

    template <class... Args>
    void console_print(const msg_side arg1, const msg_type arg2, Args... args) {
        auto [out, size] = fmt::format_to_n(fmt_buffer, FMT_BUFFER_SIZE - 1, args...);
        *out = '\0';
        scope::printing(arg1, arg2);
    }
};