#include "Mesh.hpp"

//=====>>> Constructor y Destructor
Mesh::Mesh() : u_camera(0), u_cameraPos(0), u_model(0),
    u_lightColor(0), u_lightPos(0) {}

Mesh::~Mesh() {}

//=====>>> Funciones Privadas

//=====>>> Funciones
void Mesh::loadMesh(const char* file_path) {
    vertex_buff.load_from_file(file_path);

    struct {
        unsigned char pos{0}, uvs{0}, norm{0}, tex{0};
    } types;
    struct {
        unsigned long long tex0_start{0}, tex0_offset{0};
        unsigned long long tex1_start{0}, tex1_offset{0};
        unsigned long long vert_start{0}, vert_offset{0};
        unsigned long long indi_start{0}, indi_offset{0};
    } ranges;

    std::memcpy(&types, vertex_buff.data, sizeof(types));
    std::memcpy(&ranges, vertex_buff.data + sizeof(types), sizeof(ranges));


    shader.initShader("shaders/model.vert", "shaders/model.frag");
    u_camera =      shader.getUniformLocation("u_Camera");
    u_cameraPos =   shader.getUniformLocation("u_CameraPos");
    u_model =       shader.getUniformLocation("u_Model");
    u_lightColor =  shader.getUniformLocation("u_LightColor");
    u_lightPos =    shader.getUniformLocation("u_LightPos");


    ranges.tex0_offset = ranges.tex0_offset > 31 ? 31 : ranges.tex0_offset;
    ranges.tex1_offset = ranges.tex1_offset > 31 ? 31 : ranges.tex1_offset;

    diffuse.loadImage(vertex_buff.data + ranges.tex0_start,
        GL_RGBA8, GL_RGBA,
        GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT);
    specular.loadImage(vertex_buff.data + ranges.tex1_start,
        GL_R8, GL_RGBA,
        GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT);


    object.initObject(
        (float*)(vertex_buff.data + ranges.vert_start), ranges.vert_offset,
        (GLuint*)(vertex_buff.data + ranges.indi_start), ranges.indi_offset);
    object.setAttributes(ATTR_POSITION, types.pos, ATTR_TEXCOORD, types.uvs, ATTR_NORMAL, types.norm);


    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    shader.setUniformVec4f(u_lightColor, lightColor);
    shader.setUniformVec3f(u_lightPos, lightPos);
    shader.setUniform1i(shader.getUniformLocation("u_Diffuse_0"), 0);
	shader.setUniform1i(shader.getUniformLocation("u_Specular_0"), 1);


    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(
        0.0f,
        -0.029999999329447746f,
        0.0f
    ));
    glm::mat4 rot = glm::mat4_cast(glm::quat(
        0.7071068286895752f,
        0.0f,
        0.0f,
        0.7071068286895752f
    ));
    glm::mat4 sca = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f));
    shader.setUniformMat4f(u_model, trans * -rot * sca);

    // TODO: Hacer un personaje tipo '256 fes', el cual tendra una especie de caja
    // lleno de algo que caracterise a C++ (pensar en idea)
}

void Mesh::render() const {
    shader.setUniformVec3f(u_cameraPos, camera.position);
    shader.setUniformMat4f(u_camera, camera.matrix);

    shader.bind();
    diffuse.bind(0);
    specular.bind(1);

    object.render();
}