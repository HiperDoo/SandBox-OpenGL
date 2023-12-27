#pragma once
#include "Global.hpp"
#include "VertexObject.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"

class Terrain {
protected:
    /*VertexObjIndex object;
    Texture2D texture;

    #define TERRAIN_SIZE 800.0f
    #define VERTEX_COUNT 128
    float x, z;*/

    VertexObjIndex object;
    Shader shader;

    GLint u_camera;
    GLint u_minHeight;
    GLint u_maxHeight;

    float* height_map_2D;
    
    uint32_t map_size;
    float map_scale;

public:
    Terrain();
    ~Terrain();

    void read_file_terrain(const char* file_path);

    void fill_map_terrain(const uint32_t min_height, const uint32_t max_height);

    void render() const;
};

class FaultFormation : public Terrain {
private:
    struct TerraintPoints {
        int x{0}, z{0};

        bool is_equal(const TerraintPoints& p) const {
            return (x == p.x) && (z == p.z);
        }
    };

public:
    FaultFormation();
    ~FaultFormation();

    void create_fault_formation(const uint32_t map_size, const uint32_t iterations,
        const float min_height, const float max_height);
    void random_points(TerraintPoints& p1, TerraintPoints& p2);
    void normalize(const float min_height, const float max_height);
};