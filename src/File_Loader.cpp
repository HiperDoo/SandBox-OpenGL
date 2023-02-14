#include "File_Loader.hpp"

namespace io {
    template <int T>
    void File_Buffer<T>::load_from_file(const char* file_path) {
        std::ifstream file(file_path, std::ifstream::binary);
        if (!file.is_open()) {
            auto error = std::system_error(errno, std::system_category());
            cmd::console_print(cmd::server, cmd::error,
                "Falla al abrir '{}': {}.", file_path, error.what());
            throw 1;
        }

        file.seekg(0, std::ios::end);
        size = file.tellg();
        if (size > T) {
            cmd::console_print(cmd::server, cmd::error,
                "El archivo '{}' debe ser menor que {} bytes.", file_path, T);
            throw 1;
        }
        file.seekg(0);
        file.read(data, size);
        data[size] = '\0';

        file.close();
    }

    template <int T>
    File_Buffer<T>::File_Buffer() { data = new char[T]; }

    template <int T>
    File_Buffer<T>::~File_Buffer() { if (!data) { delete[] data; } }

    template <int T>
    void File_Buffer<T>::destroyBuffer() { if (!data) { delete[] data; } }
};

template struct io::File_Buffer<32 * ONE_MB>;
template struct io::File_Buffer<16 * ONE_KB>;