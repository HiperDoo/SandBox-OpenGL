#include "Mesh.hpp"

//=====>>> Constructor y Destructor
Mesh::Mesh() : u_camera(0), u_cameraPos(0), u_model(0),
    u_lightColor(0), u_lightPos(0) {}

Mesh::~Mesh() {}

//=====>>> Funciones
void Mesh::loadMesh(const char* file_path) {
    struct {
        unsigned char pos{0}, uvs{0}, norm{0}, tex{0};
    } types;
    struct {
        unsigned long long tex0_start{0}, tex0_offset{0};
        unsigned long long tex1_start{0}, tex1_offset{0};
        unsigned long long vert_start{0}, vert_offset{0};
        unsigned long long indi_start{0}, indi_offset{0};
    } ranges;


    // Cargar configuracion del modelo
    vertex_buff.load_from_file(file_path);
    std::memcpy(&types, vertex_buff.data, sizeof(types));
    std::memcpy(&ranges, vertex_buff.data + sizeof(types), sizeof(ranges));


    // Inicializar shaders
    shader.initShader(
        "shaders/model.vert", "shaders/model.frag"
        #ifdef HARDCODE_NO_SPECULAR_MAP
        ,shader_buff.add_defines_to_shader("#define NO_SPECULAR\n")
        #endif
    );
    u_camera =      shader.getUniformLocation("u_Camera");
    u_cameraPos =   shader.getUniformLocation("u_CameraPos");
    u_model =       shader.getUniformLocation("u_Model");
    u_lightColor =  shader.getUniformLocation("u_LightColor");
    u_lightPos =    shader.getUniformLocation("u_LightPos");


    // Textura del Modelo
    diffuse.loadImage(vertex_buff.data + ranges.tex0_start,
        GL_RGBA8, GL_RGBA,
        GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT);

    // Textura Specular Map
    #ifndef HARDCODE_NO_SPECULAR_MAP
    specular.loadImage(vertex_buff.data + ranges.tex1_start,
        GL_R8, GL_RGBA,
        GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT);
    #endif


    // Inicializar vertices del objeto
    object.initObject(
        (float*)(vertex_buff.data + ranges.vert_start), ranges.vert_offset,
        (GLuint*)(vertex_buff.data + ranges.indi_start), ranges.indi_offset);
    object.setAttributes(ATTR_POSITION, types.pos, ATTR_TEXCOORD, types.uvs, ATTR_NORMAL, types.norm);
}

void Mesh::set_light_and_position(const glm::vec4& lightColor,
    const glm::vec3& lightPos, const glm::mat4& objectModel) {
    shader.setUniformMat4f(u_model, objectModel);
    shader.setUniformVec4f(u_lightColor, lightColor);
    shader.setUniformVec3f(u_lightPos, lightPos);

    shader.setUniform1i(shader.getUniformLocation("u_Diffuse_0"), 0);
    #ifndef HARDCODE_NO_SPECULAR_MAP
	shader.setUniform1i(shader.getUniformLocation("u_Specular_0"), 1);
    #endif
}

void Mesh::render() const {
    shader.setUniformVec3f(u_cameraPos, camera.position);
    shader.setUniformMat4f(u_camera, camera.matrix);

    shader.bind();
    diffuse.bind(0);
    #ifndef HARDCODE_NO_SPECULAR_MAP
    specular.bind(1);
    #endif

    object.render();
}