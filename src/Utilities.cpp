#include "Utilities.hpp"

//=============
// FILE MANAGER
//=============
namespace io {
    //=====>>> Constructor y Destructor
    template <int T>
    File_Buffer<T>::File_Buffer() {
        data = new char[T];
        if (T == _16_KB) std::memcpy(data, SHADER_HEADER, sizeof(SHADER_HEADER) - 1);
    }

    template <int T>
    File_Buffer<T>::~File_Buffer() { if (!data) { delete[] data; } }

    //=====>>> Funciones
    template <int T>
    void File_Buffer<T>::load_from_file(const char* file_path, const unsigned int offset) {
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
        if (T == _16_KB) {
            constexpr unsigned int header = sizeof(SHADER_HEADER) - 1;
            file.read(data + header + offset, size);
            data[size + header + offset] = '\0';
        } else {
            file.read(data, size);
            data[size] = '\0';
        }

        file.close();
    }

    template <int T>
    void File_Buffer<T>::destroyBuffer() { if (!data) { delete[] data; } }

    template class File_Buffer<_16_KB>;
    template class File_Buffer<_32_MB>;
};


//==============
// PRINT MANAGER
//==============
namespace cmd {
    char fmt_buffer[FMT_BUFFER_SIZE];

    namespace scope {
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
    }
};