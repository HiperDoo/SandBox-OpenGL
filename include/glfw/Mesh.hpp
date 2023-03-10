#ifndef MESH_HPP
#define MESH_HPP

#include "Global.hpp"
#include "VertexObject.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"

class Mesh {
private:
    //=====>>> Variables Privadas
    VertexObjIndex object;
    Texture2D diffuse, specular;
    Shader shader;
    GLint u_camera, u_cameraPos, u_model, u_lightColor, u_lightPos;

    #define HARDCODE_NO_SPECULAR_MAP

public:
    //=====>>> Constructor y Destructor
    Mesh();
    ~Mesh();

    //=====>>> Funciones
    void loadMesh(const char* file_path);
    void set_light_and_position(const glm::vec4& lightColor, const glm::vec3& lightPos,
        const glm::mat4& objectModel);

    void render() const;
};

#endif