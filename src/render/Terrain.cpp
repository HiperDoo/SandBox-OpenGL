#include "Terrain.hpp"


/*Terrain::Terrain(int gridX, int gridY) :
    x(gridX * TERRAIN_SIZE), z(gridY * TERRAIN_SIZE) {}

Terrain::~Terrain() {}

//=====>>> Funciones
void Terrain::generateTerrain() {
    //int count = VERTEX_COUNT * VERTEX_COUNT;
    float* vertices = new float(count * 3);
    float* normals = new float(count * 3);
    float* texcoords = new float(count * 2);
    float* vertices = new float(VERTEX_COUNT * VERTEX_COUNT * 8);
    GLuint* indices = new GLuint[6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1)];

    int vertex_ptr{0};
    for (int i{0}; i < VERTEX_COUNT; i++) {
        for (int j{0}; j < VERTEX_COUNT; j++) {
            vertices[vertex_ptr * 3] =          j / (VERTEX_COUNT - 1) * TERRAIN_SIZE;
            vertices[(vertex_ptr * 3) + 1] =    0;
            vertices[(vertex_ptr * 3) + 2] =    i / (VERTEX_COUNT - 1) * TERRAIN_SIZE;

            normals[vertex_ptr * 3] =       0;
            normals[(vertex_ptr * 3) + 1] = 1;
            normals[(vertex_ptr * 3) + 2] = 0;

            texcoords[(vertex_ptr * 3) + 2] = j / (VERTEX_COUNT - 1);
            texcoords[(vertex_ptr * 3) + 1] = i / (VERTEX_COUNT - 1);
            vertices[vertex_ptr + 0] = j / (VERTEX_COUNT - 1) * TERRAIN_SIZE;
            vertices[vertex_ptr + 1] = 0;
            vertices[vertex_ptr + 2] = i / (VERTEX_COUNT - 1) * TERRAIN_SIZE;

            vertices[vertex_ptr + 3] = 0;
            vertices[vertex_ptr + 4] = 1;
            vertices[vertex_ptr + 5] = 0;

            vertices[vertex_ptr + 6] = j / (VERTEX_COUNT - 1);
            vertices[vertex_ptr + 7] = i / (VERTEX_COUNT - 1);

            vertex_ptr += 8;
        }
    }

    int ptr{0};
    for (int gz{0}; gz < VERTEX_COUNT - 1; gz++) {
        for (int gx{0}; gx < VERTEX_COUNT - 1; gx++) {
            int topLeft = gx + (gz * VERTEX_COUNT);
            int topRight = topLeft + 1;
            int bottomLeft = gx + ((gz + 1) * VERTEX_COUNT);
            int bottomRight = bottomLeft + 1;

            indices[ptr++] = topLeft;
            indices[ptr++] = bottomLeft;
            indices[ptr++] = topRight;
            indices[ptr++] = topRight;
            indices[ptr++] = bottomLeft;
            indices[ptr++] = bottomRight;
        }
    }

    object.initObject(
        vertices, VERTEX_COUNT * VERTEX_COUNT * 8,
        indices, 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1)
    );

    delete[] vertices;
    //delete[] normals;
    //delete[] texcoords;
    delete[] indices;
}*/

//=====>>> Constructor y Destructor
Terrain::Terrain()
    : height_map_2D(nullptr), terrain_size(0), world_scale(4.0f) {}

Terrain::~Terrain() {
    if (height_map_2D) delete[] height_map_2D;
}

void Terrain::load_terrain(const char* file_path) {
    // Cargar terreno del archivo.
    const size_t size = general_buff.load_from_file(file_path);

    // Calculamos tamano del terreno.
    terrain_size = sqrtf(size / sizeof(float));

    height_map_2D = new float[terrain_size * terrain_size];
    std::memcpy(height_map_2D, general_buff.data, size);

    float* flat_mesh = new float[terrain_size * terrain_size * 3];
    int cnt{0};
    for (uint32_t i{0}; i < terrain_size; i++) {
        for (uint32_t j{0}; j < terrain_size; j++) {
            flat_mesh[cnt++] = j * world_scale;
            flat_mesh[cnt++] = height_map_2D[(terrain_size * i) + j];
            flat_mesh[cnt++] = i * world_scale;
        }
    }

    const uint32_t num_quads = (terrain_size - 1) * (terrain_size - 1);
    uint32_t* indices = new uint32_t[num_quads * 6];

    cnt = 0;
    for (uint32_t z{0}; z < terrain_size - 1; z++) {
        for (uint32_t x{0}; x < terrain_size - 1; x++) {
            uint32_t bottom_left =  x +         (z *        terrain_size);
            uint32_t top_left =     x +         ((z + 1) *  terrain_size);
            uint32_t top_right =    (x + 1) +   ((z + 1) *  terrain_size);
            uint32_t bottom_right = (x + 1) +   (z *        terrain_size);

            // Triangulo izquierdo
            indices[cnt++] = bottom_left;
            indices[cnt++] = top_left;
            indices[cnt++] = top_right;

            // Triangulo derecho
            indices[cnt++] = bottom_left;
            indices[cnt++] = top_right;
            indices[cnt++] = bottom_right;
        }
    }

    object.initObject(
        flat_mesh, terrain_size * terrain_size * 12,
        indices, num_quads * 6 * 4
    );
    object.setAttributes(ATTR_POSITION, 3);

    shader.initShader("shaders/terrain.vert", "shaders/terrain.frag");
    u_camera = shader.getUniformLocation("u_Camera");

    delete[] flat_mesh;
    delete[] indices;
}

void Terrain::render() const {
    shader.setUniformMat4f(u_camera, camera.matrix);
    shader.bind();
    object.render();
}