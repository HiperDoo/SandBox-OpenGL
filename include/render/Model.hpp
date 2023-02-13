#ifndef MODEL_H
#define MODEL_H

#include <json/json.hpp>
#include "Global.hpp"

using json = nlohmann::json;

struct Vertex {
    struct ptn {
        float position[3];
        float texUV[2];
        float normal[3];
    };
    union {
        ptn u;
        float v[8];
    };
};

class Model {
private:
    //=====>>> Variables Privadas
    std::vector<unsigned char> data;
    const char* file_path;
    json JSON;

    std::vector<std::string> loadedTexName;

    std::vector<glm::vec3> translationsMeshes;
    std::vector<glm::quat> rotationsMeshes;
    std::vector<glm::vec3> scalesMeshes;

    //=====>>> Funciones Privados
    std::string get_file_contents(const char* file_path);
    std::vector<Vertex> assembleVertices(
        std::vector<glm::vec3> positions,
        std::vector<glm::vec2> texUVs,
	    std::vector<glm::vec3> normals
    );

public:
    //=====>>> Constructor y Destructor
    Model();
    ~Model();

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<std::string> textures;

    std::vector<glm::mat4> matricesMeshes;

    void initModel(const char* file_path);

    std::vector<unsigned char> getData();
    std::vector<float> getFloats(json accessor);
    std::vector<GLuint> getIndices(json accessor);
    std::vector<std::string> getTextures();

    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);

    void loadMesh(unsigned int indMesh);
    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

    //=====>>> Funciones
    void render() const;
};

#endif