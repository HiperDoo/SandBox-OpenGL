#include "Mesh.hpp"

//=====>>> Constructor y Destructor
template <int T> Mesh<T>::Mesh() : u_camera(0), u_cameraPos(0),
    u_model(0), u_lightColor(0), u_lightPos(0), matrix(1.0f),
    position(0.0f), rotation(1.0f), scale(1.0f) {}

template <int T> Mesh<T>::~Mesh() {}

//=====>>> Funciones
template <int T>
void Mesh<T>::loadMesh(const char* file_path, const char* vertex_path, const char* fragment_path,
    const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sca) {
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
        vertex_path, fragment_path
        #ifdef HARDCODE_NO_SPECULAR_MAP
        ,shader_buff.add_defines_to_shader("#define NO_SPECULAR\n")
        #endif
    );
    u_camera =      shader.getUniformLocation("u_Camera");
    u_cameraPos =   shader.getUniformLocation("u_CameraPos");
    u_model =       shader.getUniformLocation("u_Model");
    u_lightColor =  shader.getUniformLocation("u_LightColor");
    u_lightPos =    shader.getUniformLocation("u_LightPos");


    if (T == 1) { // Textura del Modelo
        diffuse.loadImage(vertex_buff.data + ranges.tex0_start,
            GL_RGBA8, GL_RGBA,
            GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT);
    } else if (T == 2) { // Textura Specular Map
        specular.loadImage(vertex_buff.data + ranges.tex1_start,
            GL_R8, GL_RGBA,
            GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT);
    }


    // Inicializar vertices del objeto
    object.initObject(
        (float*)(vertex_buff.data + ranges.vert_start), ranges.vert_offset,
        (GLuint*)(vertex_buff.data + ranges.indi_start), ranges.indi_offset);
    if (T == 0) object.setAttributes(ATTR_POSITION, types.pos, ATTR_NORMAL, types.norm);
    else        object.setAttributes(ATTR_POSITION, types.pos, ATTR_TEXCOORD, types.uvs, ATTR_NORMAL, types.norm);


    // Transformar matrix del modelo
    position = pos; rotation = rot; scale = sca;
    matrix = glm::translate(matrix, pos);
    matrix = glm::rotate(matrix, glm::radians(0.0f), rot);
    matrix = glm::scale(matrix, sca);
    shader.setUniformMat4f(u_model, matrix);
}

template <int T>
void Mesh<T>::setLight(const glm::vec3& lightColor, const glm::vec3& lightPos) {
    shader.setUniformVec3f(u_lightColor, lightColor);
    shader.setUniformVec3f(u_lightPos, lightPos);

    if (T == 1)         shader.setUniform1i(shader.getUniformLocation("u_Diffuse_0"), 0);
    else if (T == 2)    shader.setUniform1i(shader.getUniformLocation("u_Specular_0"), 1);
}

template <int T>
void Mesh<T>::render() const {
    shader.setUniformMat4f(u_model, matrix);
    shader.setUniformVec3f(u_cameraPos, camera.position);
    shader.setUniformMat4f(u_camera, camera.matrix);

    shader.bind();
    if (T == 1)         diffuse.bind(0);
    else if (T == 2)    specular.bind(1);

    object.render();
}

template class Mesh<0>;
template class Mesh<1>;
template class Mesh<2>;