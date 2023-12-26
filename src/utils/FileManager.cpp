#include "FileManager.hpp"

#include "LogManager.hpp"
#include <fstream>

namespace io {
    //=====>>> Constructor y Destructor
    template <int T>
    FileBuffer<T>::FileBuffer() {
        data = new char[T];
        // Agregar un header al buffer de shaders solamente.
        if (T == _16_KB) std::memcpy(data, SHADER_HEADER, sizeof(SHADER_HEADER) - 1);
    }

    template <int T>
    FileBuffer<T>::~FileBuffer() { if (!data) { delete[] data; } }

    //=====>>> Funciones
    template <int T>
    size_t FileBuffer<T>::load_from_file(const char* file_path, const unsigned int offset) {
        std::ifstream file(file_path, std::ifstream::binary);
        if (!file.is_open()) {
            auto error = std::system_error(errno, std::system_category());
            cmd::console_print(cmd::client, cmd::error,
                "Falla al abrir '{}': {}.",
                file_path, error.what()
            );
            throw 1;
        }

        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        if (size > T) {
            cmd::console_print(cmd::client, cmd::error,
                "El archivo '{}' debe ser menor que {} bytes.",
                file_path, T
            );
            throw 1;
        }
        file.seekg(0);

        if (T == _16_KB) {
            constexpr unsigned int header = sizeof(SHADER_HEADER) - 1;
            file.read(data + header + offset, size);
            data[size + header + offset] = '\0';
        } else {
            file.read(data, size);
            data[size] = '\0';
        }

        file.close();
        return size;
    }

    template <int T>
    void FileBuffer<T>::destroyBuffer() { if (!data) { delete[] data; } }
};

template class io::FileBuffer<_16_KB>;
template class io::FileBuffer<_32_MB>;