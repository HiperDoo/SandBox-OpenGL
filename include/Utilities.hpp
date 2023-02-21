#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "Log.hpp"
#include <fstream>

#define ONE_KB 1024
#define ONE_MB ONE_KB * ONE_KB

#define SHADER_BUFF 0
#define VERTEX_BUFF 1

namespace io {
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

namespace ms {
    template <class DT = std::chrono::milliseconds,
        class ClockT = std::chrono::steady_clock>
    struct Timer {
        using timep_t = typename ClockT::time_point;
        timep_t start = ClockT::now();
        timep_t end = {};

        void tick() { 
            end = timep_t{};
            start = ClockT::now();
        }
        
        void tock() { end = ClockT::now(); }
        
        template <class T = DT>
        auto duration() const {
            return std::chrono::duration_cast<T>(end - start);
        }
    };
};

#endif