#include "Resources.hpp"

Skybox skybox;
Cpp_img cpp_img;
Piramid piramid;
Wood_Floor w_floor;
Light light;

Objects objects;

void initResources() {
    /// NOTE: https://tools.wwwtyro.net/space-3d/index.html#animationSpeed=1.9679712676194931&fov=94.95075971890812&nebulae=true&pointStars=true&resolution=512&seed=6ahs2zqo8e80&stars=true&sun=true
    const char* skybox_file_paths[6] = {
        "img/galaxy/right.jpg", "img/galaxy/left.jpg",
        "img/galaxy/top.jpg",   "img/galaxy/bottom.jpg",
        "img/galaxy/front.jpg", "img/galaxy/back.jpg",
    };

    constexpr GLfloat skybox_vert[] = {
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f
    };

    constexpr GLuint skybox_indi[] = {
        1, 2, 6,
        6, 5, 1,

        0, 4, 7,
        7, 3, 0,

        4, 5, 6,
        6, 7, 4,

        0, 3, 2,
        2, 1, 0,

        0, 1, 5,
        5, 4, 0,

        3, 7, 6,
        6, 2, 3
    };

    constexpr float square = 0.75f;
    constexpr GLfloat cpp_vert[] = {
        -square, -square, 0.0f, 0.0f,
         square, -square, 1.0f, 0.0f,
         square,  square, 1.0f, 1.0f,
        -square,  square, 0.0f, 1.0f
    };

    constexpr GLuint cpp_indi[] = {
        0, 1, 2,
        2, 3, 0
    };

    constexpr GLfloat piramid_vert[] = {
        // COORDINATES          / TEXCOORDS      / NORMALS
        -0.5f, 0.0f,  0.5f,     0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
        -0.5f, 0.0f, -0.5f,     0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
         0.5f, 0.0f, -0.5f,     5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
         0.5f, 0.0f,  0.5f,     5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

        -0.5f, 0.0f,  0.5f,     0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
        -0.5f, 0.0f, -0.5f,     5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
         0.0f, 0.8f,  0.0f,     2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

        -0.5f, 0.0f, -0.5f,     5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
         0.5f, 0.0f, -0.5f,     0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
         0.0f, 0.8f,  0.0f,     2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

         0.5f, 0.0f, -0.5f,     0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
         0.5f, 0.0f,  0.5f,     5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
         0.0f, 0.8f,  0.0f,     2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

         0.5f, 0.0f,  0.5f,     5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
        -0.5f, 0.0f,  0.5f,     0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
         0.0f, 0.8f,  0.0f,     2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
    };

    constexpr GLuint piramid_indi[] = {
        0, 1, 2,    // Bottom side
        0, 2, 3,    // Bottom side
        4, 6, 5,    // Left side
        7, 9, 8,    // Non-facing side
        10, 12, 11, // Right side
        13, 15, 14  // Facing side
    };

    constexpr GLfloat w_floor_vert[] = {
        // COORDINATES          / TEXCOORDS     / NORMALS
        -1.0f, 0.0f,  1.0f,     0.0f, 0.0f,     0.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, -1.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
         1.0f, 0.0f, -1.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
         1.0f, 0.0f,  1.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
    };

    constexpr GLuint w_floor_indi[] = {
        0, 1, 2,
        0, 2, 3
    };

    constexpr GLfloat light_vert[] = {
        -0.1f, -0.1f,  0.1f,
        -0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, -0.1f,
         0.1f, -0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f, -0.1f,
         0.1f,  0.1f, -0.1f,
         0.1f,  0.1f,  0.1f
    };

    constexpr GLuint light_indi[] = {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };


    ////////// Inicializacion de todos los shaders (tanto vertex como fragment shaders)
    skybox.shader.initShader("shaders/skybox.vert", "shaders/skybox.frag");
    cpp_img.shader.initShader("shaders/cpp.vert", "shaders/cpp.frag");
    piramid.shader.initShader("shaders/piramid.vert", "shaders/piramid.frag");
    w_floor.shader.initShader("shaders/floor.vert", "shaders/floor.frag");
    light.shader.initShader("shaders/light.vert", "shaders/light.frag");


    // Se obtienen todos los uniforms (variables globales que se usan dentro de los shaders),
	// para tener un facil acceso directo.
    skybox.u_view = skybox.shader.getUniformLocation("u_View");
    skybox.u_projection = skybox.shader.getUniformLocation("u_Projection");

    cpp_img.u_camera = cpp_img.shader.getUniformLocation("u_Camera");

    piramid.u_camera = piramid.shader.getUniformLocation("u_Camera");
    piramid.u_cameraPos = piramid.shader.getUniformLocation("u_CameraPos");
    piramid.u_model = piramid.shader.getUniformLocation("u_Model");
    piramid.u_lightColor = piramid.shader.getUniformLocation("u_LightColor");
    piramid.u_lightPos = piramid.shader.getUniformLocation("u_LightPos");

    w_floor.u_camera = w_floor.shader.getUniformLocation("u_Camera");
    w_floor.u_cameraPos = w_floor.shader.getUniformLocation("u_CameraPos");
    w_floor.u_model = w_floor.shader.getUniformLocation("u_Model");
    w_floor.u_lightColor = w_floor.shader.getUniformLocation("u_LightColor");
    w_floor.u_lightPos = w_floor.shader.getUniformLocation("u_LightPos");

    light.u_camera = light.shader.getUniformLocation("u_Camera");
    light.u_model = light.shader.getUniformLocation("u_Model");
    light.u_lightColor = light.shader.getUniformLocation("u_LightColor");


    ////////// Inicializacion de todas las texturas (obtenidas de sus respectivos archivos)
    skybox.texture.loadImagesCubeMap(skybox_file_paths,
        GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);

    cpp_img.texture.loadImage("img/cpp.png",
        GL_RGBA8, GL_RGBA,
        GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);

    piramid.texture.loadImage("img/brick.png",
        GL_RGBA8, GL_RGBA,
        GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT);

    w_floor.texture.loadImage("img/planks.png",
        GL_RGBA8, GL_RGBA,
        GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT);
    w_floor.specular.loadImage("img/planksSpec.png",
        GL_R8, GL_RGBA,
        GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT);


    ////////// Inicializacion de todos los objetos de vertices
    skybox.vertexObj.initObject(
        skybox_vert, sizeof(skybox_vert),
        skybox_indi, sizeof(skybox_indi));
    skybox.vertexObj.setAttributes(ATTR_POSITION, 3);

    cpp_img.vertexObj.initObject(
        cpp_vert, sizeof(cpp_vert),
        cpp_indi, sizeof(cpp_indi));
    cpp_img.vertexObj.setAttributes(ATTR_POSITION, 2, ATTR_TEXCOORD, 2);

    piramid.vertexObj.initObject(
        piramid_vert, sizeof(piramid_vert),
        piramid_indi, sizeof(piramid_indi));
    piramid.vertexObj.setAttributes(ATTR_POSITION, 3, ATTR_TEXCOORD, 2, ATTR_NORMAL, 3);

    w_floor.vertexObj.initObject(
        w_floor_vert, sizeof(w_floor_vert),
        w_floor_indi, sizeof(w_floor_indi));
    w_floor.vertexObj.setAttributes(ATTR_POSITION, 3, ATTR_TEXCOORD, 2, ATTR_NORMAL, 3);

    light.vertexObj.initObject(
        light_vert, sizeof(light_vert),
        light_indi, sizeof(light_indi));
    light.vertexObj.setAttributes(ATTR_POSITION, 3);


    ////////// Inicializacion de meshes (objetos)
    // TODO: Hacer un personaje tipo '256 fes', el cual tendra una especie de caja
    // lleno de cosas que caracterise a C++ (mejorar idea...)
    objects.init(1, 2, 0);
    objects.m_Common[0].loadMesh(
        "models/sword/dragon.bin",
        "shaders/test2.vert", "shaders/test2.frag",
        glm::vec3(5.0f, 0.0f, 0.0f),    // Posicion
        glm::vec3(1.0f),                // Rotacion
        glm::vec3(0.5f)                 // Escala
    );
    objects.m_UVs[0].loadMesh(
        "models/sword/sword.bin",
        "shaders/model.vert", "shaders/model.frag",
        glm::vec3(0.0f, 0.0f, -2.0f),
        glm::vec3(1.0f), 
        glm::vec3(0.1f)
    );
    objects.m_UVs[1].loadMesh(
        "models/sword/prism.bin",
        "shaders/test.vert", "shaders/test.frag",
        glm::vec3(0.0f, 0.0f, -2.0f),
        glm::vec3(1.0f),
        glm::vec3(1.0f)
    );


    ////////// Inicializacion de uniforms constantes
    // TODO: mejorar el sistema de posicionamiento de objetos, demasiado hardcode aqui
    glm::vec4 lightColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);

	glm::mat4 floorModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

    // Skybox
    skybox.shader.setUniform1i(skybox.shader.getUniformLocation("u_Cube"), 0);
    cpp_img.shader.setUniform1i(cpp_img.shader.getUniformLocation("u_Texture"), 0);

    // Light
    light.shader.setUniformMat4f(light.u_model, lightModel);
    light.shader.setUniformVec4f(light.u_lightColor, lightColor);

    // Piramid
    piramid.shader.setUniformMat4f(piramid.u_model, floorModel);
    piramid.shader.setUniformVec4f(piramid.u_lightColor, lightColor);
    piramid.shader.setUniformVec3f(piramid.u_lightPos, lightPos);

    // Floor
    w_floor.shader.setUniformMat4f(w_floor.u_model, floorModel);
    w_floor.shader.setUniformVec4f(w_floor.u_lightColor, lightColor);
    w_floor.shader.setUniformVec3f(w_floor.u_lightPos, lightPos);

    w_floor.shader.setUniform1i(w_floor.shader.getUniformLocation("u_Texture_0"), 0);
	w_floor.shader.setUniform1i(w_floor.shader.getUniformLocation("u_Texture_1"), 1);

    // Models
    objects.m_Common[0].setLight(glm::vec3(lightColor.x, lightColor.y, lightColor.z), lightPos);
    objects.m_UVs[0].setLight(glm::vec3(lightColor.x, lightColor.y, lightColor.z), lightPos);
    objects.m_UVs[1].setLight(glm::vec3(lightColor.x, lightColor.y, lightColor.z), lightPos);

    objects.init(0, 1, 0);
    objects.m_UVs[0].loadMesh(
        "models/sword/dragon.bin",
        "shaders/simple.vert", "shaders/simple.frag",
        glm::vec3(0.0f, -0.75f, -2.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.2f)
    );
    objects.m_UVs[0].setLight(
        glm::vec3(1.0f, 0.0f, 0.0f), // Color
        glm::vec3(0.0f, 0.0f, 50.0f) // Posicion
    );
}