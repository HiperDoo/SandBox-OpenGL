#pragma once
#include <cstring>

namespace io {
    #define SHADER_HEADER "#version 450 core\n"

    #define _16_KB 16 * 1024
    #define _32_MB 32 * 1024 * 1024

    template <int T>
    class FileBuffer {
    public:
        char* data;

        FileBuffer();
        ~FileBuffer();

        size_t load_from_file(const char* file_path, const unsigned int offset = 0);
        void destroyBuffer();

        // Función solamente para buffer de shaders.
        template <class... D>
        unsigned int add_defines_to_shader(D... defines) {
            (std::memcpy(data + sizeof(SHADER_HEADER) - 1, defines, std::strlen(defines)), ...);
            return (std::strlen(defines) + ... + 0);
        }
    };

    using Shader_Buffer =  FileBuffer<_16_KB>;
    using General_Buffer = FileBuffer<_32_MB>;
};