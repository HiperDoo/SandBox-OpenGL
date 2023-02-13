#ifndef FILE_LOADER_HPP
#define FILE_LOADER_HPP

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

#endif