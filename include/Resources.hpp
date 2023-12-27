#pragma once
#include "Mesh.hpp"
#include "Terrain.hpp"

struct Skybox {
    Shader shader;
    TextureCubeMap texture;
    VertexObjIndex vertexObj;
    GLint u_view, u_projection;
};

struct Cpp_img {
    Shader shader;
    Texture2D texture;
    VertexObjIndex vertexObj;
    GLint u_camera;
};

struct Piramid {
    Shader shader;
    Texture2D texture;
    VertexObjIndex vertexObj;
    GLint u_camera, u_cameraPos, u_model, u_lightColor, u_lightPos;
};

struct Wood_Floor {
    Shader shader;
    Texture2D texture, specular;
    VertexObjIndex vertexObj;
    GLint u_camera, u_cameraPos, u_model, u_lightColor, u_lightPos;
};

struct Light {
    Shader shader;
    VertexObjIndex vertexObj;
    GLint u_camera, u_model, u_lightColor;
};

extern Skybox skybox;
extern Cpp_img cpp_img;
extern Piramid piramid;
extern Wood_Floor w_floor;
extern Light light;

class Terrain;
class FaultFormation;
extern FaultFormation ground;


// Necesario Definir antes de instanciar (usar).
class Objects;
extern Objects objects;

void initResources();