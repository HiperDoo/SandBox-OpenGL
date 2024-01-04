#pragma once
#include "Renderer.hpp"

void MouseScrollCallback(GLFWwindow*, double posX, double posY) {
    mouse::scrollX = static_cast<int32_t>(posX);
    mouse::scrollY = static_cast<int32_t>(posY);
}

void update_game();
void fixed_update_game();
void render_game();

void init_GLFW() {
    // Temporizador para toda la inicialización de la ventana de OpenGL.
    ms::Timer t_initWindow;

    app.window_init("SandBox OpenGL | fps: ", 720, 480);
    app.camera_init(glm::vec3(0.0f, 0.0f, 2.0f), 45.0f);

    glfwSetScrollCallback(app.glfw, MouseScrollCallback);
    cmd::console_print(cmd::client, cmd::debug,
        "CREACION Y CONFIGURACION DE VENTANA OPENGL ({} secs)",
        t_initWindow.tock().count() / 1000.0f
    );

    pc::get_pc_data();
}

void run_game() {
    // Inicialización de todos los objetos/texturas.
    initResources();

    // Desalojar buffers de uso común (en este caso, ya no se volverán a usar).
    vertex_buff.destroyBuffer();
    shader_buff.destroyBuffer();
    general_buff.destroyBuffer();

    //
    ////
    ////// Main Loop de todo el programa.
    while (!glfwWindowShouldClose(app.glfw)) {
        app.window_update_clock();

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
        while (app.window_is_FixedUpdate()) {
            fixed_update_game();
            app.window_feed_FixedUpdate();
        }

        /// Frame Rate Update
        // - Zona para renderizar todo aquello pre-calculado.
        render_game();

        // Si sobra tiempo, dormir la CPU hasta el siguiente fotograma.
        //delta::sleep_thread();
    }
}

static float test_rotation{0.0f};

void update_game() {
    app.camera_inputs();
    app.camera_update_Matrix();

    test_rotation += app.dt * 60.0f;
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

    glfwSwapBuffers(app.glfw);
    glfwPollEvents();
}

void shut_down() {
    glfwTerminate();
}