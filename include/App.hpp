#pragma once
#include "Renderer.hpp"

//=================
// OPENGL CALLBACKS
//=================
void FramebufferSizeCallback(GLFWwindow *window, GLsizei width, GLsizei height) {
    window::width = static_cast<uint32_t>(width);
    window::height = static_cast<uint32_t>(height);
    window::half_width = window::width / 2;
    window::half_height = window::height / 2;

    glViewport(0, 0, window::width, window::height);
    camera.updateAspectRatio();
}

void CursorPosCallback(GLFWwindow*, double posX, double posY) {
    mouse::posX = static_cast<int32_t>(posX);
    mouse::posY = static_cast<int32_t>(posY);

    if (mouse::first_move) {
        mouse::lastX = mouse::posX;
        mouse::lastY = mouse::posY;
        mouse::first_move = false;
    }

    mouse::offsetX = mouse::posX - mouse::lastX;
    mouse::offsetY = mouse::lastY - mouse::posY;

    mouse::lastX = mouse::posX;
    mouse::lastY = mouse::posY;
}

void MouseScrollCallback(GLFWwindow*, double posX, double posY) {
    mouse::scrollX = static_cast<int32_t>(posX);
    mouse::scrollY = static_cast<int32_t>(posY);
}

void ErrorCallback(int, const char* err_str) {
    cmd::console_print(cmd::opengl, cmd::error, err_str);
    throw EXIT_FAILURE;
}

//===================
// APLICACIÓN GENERAL
//===================
void update_game();
void fixed_update_game();
void render_game();

void init_GLFW() {
    // Temporizador para toda la inicialización de la ventana de OpenGL.
    ms::Timer t_initWindow;

    // Definir que función llamar cuando un error de OpenGL se produce.
    glfwSetErrorCallback(ErrorCallback);

    // Inicialización de GLFW.
    if (glfwInit() != GLFW_TRUE) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado inicializar GLFW."
        );
        throw EXIT_FAILURE;
    }

    // Parámetros de sugerencia para la ventana a crear (es posible que una plataforma adopte diferentes parámetros).
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Ventana flotante para WMs de Linux.

    // Seteo del nombre de la ventana.
    window::name = new char[64];
    strcpy(window::name, "SandBox OpenGL | fps: 60.0");

    // Creación de la ventana OpenGL.
    window::glfw = glfwCreateWindow(window::width, window::height, window::name, nullptr, nullptr);
    if (!window::glfw) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado crear la ventana."
        );
        glfwTerminate();
        throw EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window::glfw);

    // Definir la tasa de refresco de la ventana (en este caso sin limites ya que nosotros nos encargaremos de eso manualmente).
    glfwSwapInterval(0);

    // Inicialización de GLEW (siempre después de inicializar GLFW).
    if (glewInit() != GLEW_OK) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado inicializar GLEW."
        );
        throw EXIT_FAILURE;
    }

    // Buscar cual es la tasa de refresco del monitor por defecto.
    delta::default_frame_rate = static_cast<double>(glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate);

    pc::get_pc_data();

    // Definir las funciones a llamar según cada evento que pueda producirse (callbacks).
    glfwSetFramebufferSizeCallback(window::glfw, FramebufferSizeCallback);
    //glfwSetCursorPosCallback(window::glfw, CursorPosCallback);
    glfwSetScrollCallback(window::glfw, MouseScrollCallback);

    // Define el tamaño y punto del área donde se renderiza nuestro contexto.
    glViewport(0, 0, window::width, window::height);

    cmd::console_print(cmd::client, cmd::debug,
        "CREACION Y CONFIGURACION DE VENTANA OPENGL ({} secs)",
        t_initWindow.tock().count() / 1000.0f
    );
}

static float test_rotation{0.0f};

void run_program() {
    // Inicialización de todos los objetos/texturas.
    initResources();
    // Desalojar buffers de uso común (en este caso, ya no se volverán a usar).
    vertex_buff.destroyBuffer();
    shader_buff.destroyBuffer();
    general_buff.destroyBuffer();

    // Activación de la función Blend para así renderizar imágenes/texturas con transparencia.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Respetar la posición de profundidad de cada objeto renderizado.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Evitar renderizar triángulos que no miran hacia la cámara.
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    delta::set_FPS(delta::FrameRate::VSYNC);

    //
    ////
    ////// Main Loop de todo el programa.
    while (!glfwWindowShouldClose(window::glfw)) {
        delta::update_dt();

        /// Frame Rate Update
        // - Zona para actualizar funciones cada fotograma (FPS),
        // como inputs del usuario y cálculo de posicionamiento de
        // animaciones (usando Delta Time).
        //
        // NOTA: Se ejecuta una vez cada fotograma
        // (60 FPS == 60 actualizaciones por segundo)
        update_game();

        /// Fixed Time Step [20 TPS]
        // - Loop para actualizar físicas del juego en su
        // preciso tiempo (NO usar Delta Time).
        //
        // NOTA: Se ejecuta una vez cada tick (sin importar los FPS)
        // (20 TPS == 20 actualizaciones por segundo)
        while (delta::fixedUpdate()) {
            fixed_update_game();
            delta::feedFixedUpdate();
        }

        /// Frame Rate Update
        // - Zona para renderizar todo aquello pre-calculado.
        render_game();

        // Si sobra tiempo, dormir la CPU hasta el siguiente Frame.
        delta::sleep_thread();
    }
}

void update_game() {
    camera.inputs();
    camera.updateMatrix();

    test_rotation += delta::dt * 60.0f;
    test_rotation = (test_rotation < 360.0f) ? test_rotation : 0.0f;
}

void fixed_update_game() {}

void render_game() {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //renderSkyBox();
    // renderPiramid();
    //renderFloor();
    //renderLight();
    // renderCppImg();
    //renderSword(test_rotation);
    renderer(test_rotation);

    glfwSwapBuffers(window::glfw);
    glfwPollEvents();
}

void shut_down() {
    glfwTerminate();
    if (window::name)
        delete[] window::name;
}