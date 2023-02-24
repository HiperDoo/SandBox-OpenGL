#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

#include <fstream>

//=============
// FILE MANAGER
//=============
namespace io {
    #define ONE_KB 1024
    #define ONE_MB ONE_KB * ONE_KB

    #define SHADER_BUFF 0
    #define VERTEX_BUFF 1

    template <int T>
    struct File_Buffer {
        char* data{nullptr};
        size_t size{0};

        File_Buffer();
        ~File_Buffer();

        void load_from_file(const char* file_path);
        void destroyBuffer();
    };
};

//======
// TIMER
//======
namespace ms {
    struct Timer {
        using timep_t = std::chrono::steady_clock;
        timep_t::time_point start = timep_t::now();
        timep_t::time_point end = {};

        void tick() { 
            end = timep_t::time_point{};
            start = timep_t::now();
        }
        
        auto tock() const {
            return std::chrono::duration_cast<std::chrono::milliseconds>(timep_t::now() - start);
        }
    };
};

//==============
// PRINT MANAGER
//==============
namespace cmd {
    // NOTE: https://github.com/fmtlib/fmt
    // NOTE: https://hackingcpp.com/cpp/libs/fmt.html#quick-fmt-specs
    #define APPLY_COLORS

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

#endif