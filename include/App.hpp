#ifndef APP_HPP
#define APP_HPP

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/sysinfo.h>
#endif

#include "Renderer.hpp"

#define OPENGL_VERSION_MAJOR 4
#define OPENGL_VERSION_MINOR 5

void get_cpu_info() {
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
    HTT   = EDX & AVX_POS;
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
    for(int i = 0, j = 0; i < 3; i++, j = i << 4) {
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
    cmd::console_print(cmd::server, cmd::debug, "Hardware utilizado:");
    fmt::print("   CPU: {} - [{} | {}]\n", model_name, inst[pos1 + 1], inst[pos2 + 7]);

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
    fmt::print("   RAM: {:.1f} GB\n", total_memory  / 1073741824.0f);

    delete[] vendor_id;
    delete[] model_name;
}

/*void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    screen_width = width;
    screen_height = height;
    glViewport(0, 0, screen_width, screen_height);
    //float aspect = (float)screen_width / (float)screen_height;
    //glm::mat4 ortho = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
    //test_sha.setUniformMat4f(u_MVP, ortho);
}*/

void ErrorCallback(int, const char *err_str) {
    cmd::console_print(cmd::opengl, cmd::error, err_str);
    throw EXIT_FAILURE;
}

void init_GLFW() {
    ms::Timer clock;
    glfwSetErrorCallback(ErrorCallback);

    if (!glfwInit()) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado inicializar GLFW.");
        throw EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_name = new char[32];
    strcpy(window_name, "SandBox OpenGL | fps: 60.0");
    window = glfwCreateWindow(screen_width, screen_height, window_name, nullptr, nullptr);
    if (!window) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado crear la ventana.");
        glfwTerminate();
        throw EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // 60fps

    if (glewInit() != GLEW_OK) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado inicializar GLEW.");
        throw EXIT_FAILURE;
    }

    // TODO: Ordenar el sistema de impresion de hardware (y agregar mas modulos, como la RAM)
    get_cpu_info();

    const char* versions[3] = {
        (const char*)glGetString(GL_RENDERER),
        (const char*)glGetString(GL_VERSION),
        (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)
    };
    const char* undefined = "Indefinido...";
    cmd::console_print(cmd::opengl, cmd::info, "Version de Render: {}", versions[0] ? versions[0] : undefined);
    cmd::console_print(cmd::opengl, cmd::info, "Version de OpenGL: {}", versions[1] ? versions[1] : undefined);
    cmd::console_print(cmd::opengl, cmd::info, "Version de Shader: {}", versions[2] ? versions[2] : undefined);

    // glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    glViewport(0, 0, screen_width, screen_height);
    cmd::console_print(cmd::server, cmd::debug,
        "CONFIGURACION Y CREACION DE VENTANA OPENGL ({} secs)", clock.tock().count() / 1000.0f);
}

void run_program() {
    // Inicializar objetos graficos.
    initResources();
    // Desalojar buffers.
    vertex_buff.destroyBuffer();
    shader_buff.destroyBuffer();

    // Activacion de la funcion Blend para asi renderizar imagenes/texturas con transparencia.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Respetar la posicion de profundidad de cada objeto renderizado.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Evitar renderizar triangulos que no apuntan a la camara.
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    //#define FRAME_RATE 1.0f / 60.0f
    //float rotation{0.0f};
    double timeDiff{0.0f}, currTime{0.0f}, prevTime = glfwGetTime();
    unsigned int counter{0};

    while (!glfwWindowShouldClose(window)) {
        currTime = glfwGetTime(); counter++;
        if ((timeDiff = currTime - prevTime) >= 1.0f) {
            //rotation += 0.5f;
            sprintf(window_name + 22, "%.1f", (1.0f / timeDiff) * counter);
            glfwSetWindowTitle(window, window_name);

            prevTime = currTime;
            counter = 0;
        }

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.inputs();
        camera.updateMatrix();

        renderSkyBox();
        // renderPiramid(rotation);
        renderFloor();
        renderLight();
        // renderCppImg();
        renderSword();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void shut_down() {
    glfwTerminate();
    delete[] window_name;
}

#endif