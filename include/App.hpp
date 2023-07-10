#ifndef APP_HPP
#define APP_HPP

#include "Renderer.hpp"

#define OPENGL_VERSION_MAJOR 4
#define OPENGL_VERSION_MINOR 5

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

void ErrorCallback(int, const char *err_str) {
    cmd::console_print(cmd::opengl, cmd::error, err_str);
    throw EXIT_FAILURE;
}

//===================
// APLICACION GENERAL
//===================
void update_game();
void fixed_update_game();
void render_game();

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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Ventana flotante para WMs de Linux

    window::name = new char[32];
    strcpy(window::name, "SandBox OpenGL | fps: 60.0");
    window::glfw = glfwCreateWindow(window::width, window::height, window::name, nullptr, nullptr);
    if (!window::glfw) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado crear la ventana.");
        glfwTerminate();
        throw EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window::glfw);
    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado inicializar GLEW.");
        throw EXIT_FAILURE;
    }

    delta::default_frame_rate = static_cast<double>(glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate);

    pc::get_pc_data();

    // Callbacks de OpenGL
    glfwSetFramebufferSizeCallback(window::glfw, FramebufferSizeCallback);
    //glfwSetCursorPosCallback(window::glfw, CursorPosCallback);
    glfwSetScrollCallback(window::glfw, MouseScrollCallback);

    glViewport(0, 0, window::width, window::height);
    cmd::console_print(cmd::client, cmd::debug,
        "CREACION Y CONFIGURACION DE VENTANA OPENGL ({} secs)", clock.tock().count() / 1000.0f);
}

static float test_rotation{0.0f};

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

    delta::set_FPS(delta::FrameRate::_75_FPS);

    //
    ////
    ////// Main Loop de todo el programa.
    while (!glfwWindowShouldClose(window::glfw)) {
        delta::update_dt();

        /// Frame Rate Update
        // - Zona para actualizar funciones cada fotograma (FPS),
        // como inputs del usuario y calculo de posicionamiento de
        // animaciones (usando Delta Time).
        //
        // NOTA: Se ejecuta una vez cada fotograma
        // (60 FPS == 60 actualizaciones por segundo)
        update_game();

        /// Fixed Time Step [20 TPS]
        // - Loop para actualizar fisicas del juego en su
        // presiso tiempo (NO usar Delta Time).
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

        // Si sobra tiempo, dormir CPU hasta el siguiente Frame.
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
    delete[] window::name;
}

#endif