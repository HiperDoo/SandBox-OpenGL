#include "Utilities.hpp"

#include <GL/glew.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#endif

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
            cmd::console_print(cmd::client, cmd::error,
                "Falla al abrir '{}': {}.", file_path, error.what());
            throw 1;
        }

        file.seekg(0, std::ios::end);
        size = file.tellg();
        if (size > T) {
            cmd::console_print(cmd::client, cmd::error,
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


//========================
// GET HARDWARE & SOFTWARE
//========================
namespace pc {
    void get_terminal_size(uint32_t& width) {
        #if defined(_WIN32)
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        width = (uint32_t)(csbi.srWindow.Right-csbi.srWindow.Left+1);
        //height = (uint32_t)(csbi.srWindow.Bottom-csbi.srWindow.Top+1);
        #else
        struct winsize w;
        ioctl(fileno(stdout), TIOCGWINSZ, &w);
        width = (uint32_t)(w.ws_col);
        //height = (uint32_t)(w.ws_row);
        #endif
    }

    void get_pc_data() {
        const uint32_t SSE_POS   = 0x02000000;
        const uint32_t SSE2_POS  = 0x04000000;
        const uint32_t SSE3_POS  = 0x00000001;
        const uint32_t SSE41_POS = 0x00080000;
        const uint32_t SSE42_POS = 0x00100000;
        const uint32_t AVX_POS   = 0x10000000;
        const uint32_t AVX2_POS  = 0x00000020;
        const uint32_t LVL_TYPE  = 0x0000FF00;
        const uint32_t LVL_CORES = 0x0000FFFF;

        enum CPU_Flags {
            SSE = 1,
            SSE2 = 2,
            SSE3 = 4,
            SSE41 = 8,
            SSE42 = 16,

            AVX = 256,
            AVX2 = 512
        };
        int cpu{0};

        char* vendor_id = new char[16];
        char* model_name = new char[48];
        int mNumSMT{0}, cores{0}, threads{0};
        bool HTT{false};

        uint32_t EAX, EBX, ECX, EDX;

        auto CPUID = [&EAX, &EBX, &ECX, &EDX](int funcId, int subFuncId) {
            asm volatile
                ("cpuid" : "=a" (EAX), "=b" (EBX), "=c" (ECX), "=d" (EDX)
                : "a" (funcId), "c" (subFuncId));
        };

        // Vendor ID
        CPUID(0, 0);
        *reinterpret_cast<uint32_t*>(vendor_id) = EBX;
        *reinterpret_cast<uint32_t*>(vendor_id + 4) = EDX;
        *reinterpret_cast<uint32_t*>(vendor_id + 8) = ECX;
        vendor_id[12] = '\0';
        uint32_t HFS = EAX;

        // Obtener disponibilidad de Instrucciones SSE
        CPUID(1, 0);
        HTT = EDX & AVX_POS;
        cpu |= !!(EDX & SSE_POS) << 0
            | !!(EDX & SSE2_POS) << 1
            | !!(ECX & SSE3_POS) << 2
            | !!(ECX & SSE41_POS) << 3
            | !!(ECX & SSE42_POS) << 4
            | !!(ECX & AVX_POS) << 8;

        // Obtener disponibilidad de Instrucciones AVX2
        CPUID(7, 0);
        cpu |= !!(EBX & AVX2_POS) << 9;

        // Obtener numero de Nucleos e Hilos
        for (int i = 0; i < 12; i++) vendor_id[i] = vendor_id[i] & ~32;
        if (strstr(vendor_id, "INTEL")) {
            if(HFS >= 11) {
                for (int lvl = 0; lvl < 4; lvl++) {
                    CPUID(0x0B, lvl);
                    uint32_t currLevel = (LVL_TYPE & ECX) >> 8;
                    if (currLevel == 1) mNumSMT = LVL_CORES & EBX;
                    else if (currLevel == 2) threads = LVL_CORES & EBX;
                }
                cores = threads / mNumSMT;
            } else {
                if (HFS >= 1) {
                    threads = (EBX >> 16) & 0xFF;
                    if (HFS >= 4) {
                        CPUID(4, 0);
                        cores = (1 + (EAX >> 26)) & 0x3F;
                    }
                }
                if (HTT) {
                    if (!(cores > 1)) {
                        cores = 1;
                        threads = (threads >= 2 ? threads : 2);
                    }
                } else {
                    cores = threads = 1;
                }
            }
        } else if (strstr(vendor_id, "AMD")) {
            if (HFS >= 1) {
                threads = (EBX >> 16) & 0xFF;
                CPUID(0x80000000, 0);
                if (EAX >=8) {
                    CPUID(0x80000008, 0);
                    cores = 1 + (ECX & 0xFF);
                }
            }
            if (HTT) {
                if (!(cores>1)) {
                    cores = 1;
                    threads = (threads >= 2 ? threads : 2);
                }
            } else {
                cores = threads = 1;
            }
        } else {
            cores = threads = 0;
        }

        // Obtener nombre de CPU
        for (int i = 0, j = 0; i < 3; i++, j = i << 4) {
            CPUID(i + 0x80000002, 0);
            *reinterpret_cast<uint32_t*>(model_name + j) = EAX;
            *reinterpret_cast<uint32_t*>(model_name + j + 4) = EBX;
            *reinterpret_cast<uint32_t*>(model_name + j + 8) = ECX;
            *reinterpret_cast<uint32_t*>(model_name + j + 12) = EDX;
        }
        model_name[47] = '\0';

        const char* inst[] = {
            "none", "SSE", "SSE2", "SSE3", "SSE4.1", "SSE4.2",
            "none", "AVX", "AVX2"
        };
        int pos1{0}, pos2{0};
        asm ("bsrl %1, %0" : "=r" (pos1) : "b" ((char)cpu & 0xFF));
        asm ("bsrl %1, %0" : "=r" (pos2) : "r" (cpu >> 8));

        double total_memory{0.0f};
        #ifdef _WIN32
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        total_memory = status.ullTotalPhys;
        #else
        struct sysinfo info;
        sysinfo(&info);
        total_memory = info.totalram;
        #endif

        const char* versions[3] = {
            (const char*)glGetString(GL_RENDERER),
            (const char*)glGetString(GL_VERSION),
            (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)
        };
        const char* undefined = "Indefinido :(";

        uint32_t width{0};
        get_terminal_size(width);
        width /= 2;

        const auto separator = fmt::format("{:=^{}}\n\n", "", width);

        // Imprimir especificaciones
        fmt::print("\n{:=^{}}\n", "> Hardware Information <", width);
        cmd::console_print(cmd::client, cmd::info, "CPU: {} - [{} | {}]", model_name, inst[pos1 + 1], inst[pos2 + 7]);
        cmd::console_print(cmd::client, cmd::info, "RAM: {:.1f} GB", total_memory  / 1073741824.0f);
        cmd::console_print(cmd::opengl, cmd::info, "GPU: {}", versions[0] ? versions[0] : undefined);
        fmt::print(separator);

        fmt::print("{:=^{}}\n", "> Drivers Information <", width);
        cmd::console_print(cmd::opengl, cmd::info, "Version de OpenGL: {}", versions[1] ? versions[1] : undefined);
        cmd::console_print(cmd::opengl, cmd::info, "Version de Shader: {}", versions[2] ? versions[2] : undefined);
        fmt::print(separator);

        delete[] vendor_id;
        delete[] model_name;
    }
}