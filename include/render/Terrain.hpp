#pragma once
#include "Global.hpp"
#include "VertexObject.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"

class Terrain {
private:
    /*VertexObjIndex object;
    Texture2D texture;

    #define TERRAIN_SIZE 800.0f
    #define VERTEX_COUNT 128
    float x, z;*/

    VertexObjIndex object;
    Shader shader;
    GLint u_camera;

    float* height_map_2D;
    
    uint32_t terrain_size;
    float world_scale;

public:
    Terrain();
    ~Terrain();

    void load_terrain(const char* file_path);
    
    void render() const;
};