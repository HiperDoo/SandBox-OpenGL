#ifndef APP_HPP
#define APP_HPP

#include "Renderer.hpp"
#include "Model.hpp"
#include "Mesh.hpp"

#include "File_Loader.hpp"

/*void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    screen_width = width;
    screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
    //float aspect = (float)screen_width / (float)screen_height;
    //glm::mat4 ortho = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
    //test_sha.setUniformMat4f(u_MVP, ortho);
}*/

void ErrorCallback(int, const char* err_str) {
    cmd::console_print(cmd::opengl, cmd::error,
        err_str);
    throw EXIT_FAILURE;
}

void init_GLFW() {
    if (!glfwInit()) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado inicializar GLFW.");
        throw EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(screen_width, screen_height, "Hello World", nullptr, nullptr);
    if (!window) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado crear la ventana.");
        glfwTerminate();
        throw EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //60fps

    if (glewInit() != GLEW_OK) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado inicializar GLEW.");
        throw EXIT_FAILURE;
    }

    //glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetErrorCallback(ErrorCallback);

    cmd::console_print(cmd::opengl, cmd::info, "Version de Render: {}", (const char*)glGetString(GL_RENDERER));
    cmd::console_print(cmd::opengl, cmd::info, "Version de OpenGL: {}", (const char*)glGetString(GL_VERSION));
    cmd::console_print(cmd::opengl, cmd::info, "Version de Shader: {}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    glViewport(0, 0, screen_width, screen_height);
}

void run_program() {
    initResources();

    Mesh mesh;
    mesh.loadMesh("new.bin");

    // Activacion de la funcion Blend para asi renderizar
    // imagenes/texturas con transparencia.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	// Enables Cull Facing
	//glEnable(GL_CULL_FACE);
	// Keeps front faces
	//glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	//glFrontFace(GL_CCW);

    #define FRAME_RATE 1.0f / 60.0f
    float rotation{0.0f};
    double currTime{0.0f};
    double prevTime = glfwGetTime();

    vertex_buff.destroyBuffer();
    shader_buff.destroyBuffer();

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currTime = glfwGetTime();
        if (currTime - prevTime >= FRAME_RATE) {
            rotation += 0.5f;
            prevTime = currTime;
        }

        camera.inputs();
        camera.updateMatrix();

        renderSkyBox();
        //renderPiramid(rotation);
        renderFloor();
        renderLight();
        //renderCppImg();

        mesh.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void shut_down() {
    glfwTerminate();
}

#endif

/*Model model;
    model.initModel("bunny/scene.gltf");

    VertexObjIndex vert_model;
    vert_model.initObject(
        model.vertices.data()->v, model.vertices.size() * 8 * sizeof(float),
        model.indices.data(), model.indices.size() * sizeof(GLuint)
    );
    vert_model.setAttributes(ATTR_POSITION, 3, ATTR_TEXCOORD, 2, ATTR_NORMAL, 3);
    Texture2D diffuse, specular;
    diffuse.loadImage(model.textures[0].c_str(),
        GL_RGBA8, GL_RGBA,
        GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT);
    specular.loadImage(model.textures[1].c_str(),
        GL_R8, GL_RGBA,
        GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT);
    Shader shader_model;
    //Shader::createBuffer();
    shader_model.initShader("shaders/model.vert", "shaders/model.frag");
    //Shader::destroyBuffer();

    GLint u_camera = shader_model.getUniformLocation("u_Camera");
    GLint u_cameraPos = shader_model.getUniformLocation("u_CameraPos");
    GLint u_model = shader_model.getUniformLocation("u_Model");
    GLint u_lightColor = shader_model.getUniformLocation("u_LightColor");
    GLint u_lightPos = shader_model.getUniformLocation("u_LightPos");

    GLint u_translation = shader_model.getUniformLocation("u_Translation");
    GLint u_rotation = shader_model.getUniformLocation("u_Rotation");
    GLint u_scale = shader_model.getUniformLocation("u_Scale");

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    shader_model.setUniformVec4f(u_lightColor, lightColor);
    shader_model.setUniformVec3f(u_lightPos, lightPos);
    shader_model.setUniform1i(shader_model.getUniformLocation("u_Diffuse_0"), 0);
	shader_model.setUniform1i(shader_model.getUniformLocation("u_Specular_0"), 1);*/

/*shader_model.setUniformVec3f(u_cameraPos, camera.position);
        shader_model.setUniformMat4f(u_camera, camera.matrix);

        shader_model.bind();
        diffuse.bind(0);
        specular.bind(1);

        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 rot = glm::mat4(1.0f);
        glm::mat4 sca = glm::mat4(1.0f);

        // Transform the matrices to their correct form
        trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
        rot = glm::mat4_cast(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
        sca = glm::scale(sca, glm::vec3(1.0f, 1.0f, 1.0f));

        // Push the matrices to the vertex shader
        shader_model.setUniformMat4f(u_translation, trans);
        shader_model.setUniformMat4f(u_rotation, rot);
        shader_model.setUniformMat4f(u_scale, sca);
        shader_model.setUniformMat4f(u_model, model.matricesMeshes[0]);
        vert_model.render();*/