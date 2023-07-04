#ifndef MESH_HPP
#define MESH_HPP

#include "Global.hpp"
#include "VertexObject.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"

template <int T>
class Mesh {
private:
    //=====>>> Variables Privadas
    VertexObjIndex object;
    Texture2D diffuse, specular;
    Shader shader;
    GLint u_camera, u_cameraPos, u_model, u_lightColor, u_lightPos;
    
    #define HARDCODE_NO_SPECULAR_MAP

public:
    //=====>>> Variables Publicas
    glm::mat4 matrix;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    //=====>>> Constructor y Destructor
    Mesh();
    ~Mesh();

    //=====>>> Funciones
    void loadMesh(const char* file_path, const char* vertex_path, const char* fragment_path,
        const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sca);
    void setLight(const glm::vec3& lightColor, const glm::vec3& lightPos);

    void render() const;
};


class Objects {
private:
    //=====>>> Variables Privadas
    unsigned int common, uvs, spec;

public:
    //=====>>> Variables Publicas
    Mesh<0>* m_Common;
    Mesh<1>* m_UVs;
    Mesh<2>* m_Spec;

    //=====>>> Constructor y Destructor
    Objects() : common(0), uvs(0), spec(0), m_Common(nullptr), m_UVs(nullptr), m_Spec(nullptr) {}
    ~Objects() {
        if (m_Common)   delete[] m_Common;
        if (m_UVs)      delete[] m_UVs;
        if (m_Spec)     delete[] m_Spec;
    }

    //=====>>> Funciones
    void init(unsigned int num_commons = 0, unsigned int num_uvs = 0, unsigned int num_spec = 0) {
        common = num_commons; uvs = num_uvs; spec = num_spec;

        if (common) m_Common    = new Mesh<0>[common];
        if (uvs)    m_UVs       = new Mesh<1>[uvs];
        if (spec)   m_Spec      = new Mesh<2>[spec];
    }

    inline void render() const {
        unsigned int i = 0;
        for (i = 0; i < common; i++)    m_Common[i].render();
        for (i = 0; i < uvs; i++)       m_UVs[i].render();
        for (i = 0; i < spec; i++)      m_Spec[i].render();
    }
};

#endif