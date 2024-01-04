#include "Terrain.hpp"


/*Terrain::Terrain(int gridX, int gridY) :
    x(gridX * map_size), z(gridY * map_size) {}

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
            vertices[vertex_ptr * 3] =          j / (VERTEX_COUNT - 1) * map_size;
            vertices[(vertex_ptr * 3) + 1] =    0;
            vertices[(vertex_ptr * 3) + 2] =    i / (VERTEX_COUNT - 1) * map_size;

            normals[vertex_ptr * 3] =       0;
            normals[(vertex_ptr * 3) + 1] = 1;
            normals[(vertex_ptr * 3) + 2] = 0;

            texcoords[(vertex_ptr * 3) + 2] = j / (VERTEX_COUNT - 1);
            texcoords[(vertex_ptr * 3) + 1] = i / (VERTEX_COUNT - 1);
            vertices[vertex_ptr + 0] = j / (VERTEX_COUNT - 1) * map_size;
            vertices[vertex_ptr + 1] = 0;
            vertices[vertex_ptr + 2] = i / (VERTEX_COUNT - 1) * map_size;

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
Terrain::Terrain() : u_camera(0), u_minHeight(0), u_maxHeight(0),
    height_map_2D(nullptr), map_size(0), map_scale(4.0f) {}

Terrain::~Terrain() {
    if (height_map_2D) delete[] height_map_2D;
}

//=====>>> Funciones
void Terrain::read_file_terrain(const char* file_path) {
    // Cargar terreno del archivo.
    size_t size = general_buff.load_from_file(file_path);

    // Calculamos tamano del terreno.
    map_size = (size_t)sqrtf(size / sizeof(float));

    height_map_2D = new float[map_size * map_size];
    std::memcpy(height_map_2D, general_buff.data, size);
}

void Terrain::fill_map_terrain(const uint32_t min_height, const uint32_t max_height) {
    int cnt{0};
    float* flat_mesh = new float[map_size * map_size * 3];

    for (uint32_t i{0}; i < map_size; i++) {
        for (uint32_t j{0}; j < map_size; j++) {
            flat_mesh[cnt++] = j * map_scale;
            flat_mesh[cnt++] = height_map_2D[(map_size * i) + j];
            flat_mesh[cnt++] = i * map_scale;
        }
    }

    const uint32_t num_quads = (map_size - 1) * (map_size - 1);
    uint32_t* indices = new uint32_t[num_quads * 6];

    cnt = 0;
    for (uint32_t z{0}; z < map_size - 1; z++) {
        for (uint32_t x{0}; x < map_size - 1; x++) {
            uint32_t bottom_left =  x +         (z *        map_size);
            uint32_t top_left =     x +         ((z + 1) *  map_size);
            uint32_t top_right =    (x + 1) +   ((z + 1) *  map_size);
            uint32_t bottom_right = (x + 1) +   (z *        map_size);

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
        flat_mesh, map_size * map_size * (sizeof(float) * 3),
        indices, num_quads * (sizeof(uint32_t) * 6)
    );
    object.setAttributes(ATTR_POSITION, 3);

    shader.initShader("shaders/terrain.vert", "shaders/terrain.frag");
    u_camera = shader.getUniformLocation("u_Camera");
    u_minHeight = shader.getUniformLocation("u_MinHeight");
    u_maxHeight = shader.getUniformLocation("u_MaxHeight");

    shader.setUniform1f(u_minHeight, min_height);
    shader.setUniform1f(u_minHeight, max_height);

    delete[] flat_mesh;
    delete[] indices;
}

void Terrain::render() const {
    shader.setUniformMat4f(u_camera, app.matrix);
    shader.bind();
    object.render();
}


FaultFormation::FaultFormation() {}
FaultFormation::~FaultFormation() {}

void FaultFormation::create_fault_formation(const uint32_t map_size, const uint32_t iterations,
    const float min_height, const float max_height) {
    this->map_size = map_size;
    height_map_2D = new float[map_size * map_size];
    for (uint32_t i{0}; i < map_size * map_size; i++) height_map_2D[i] = 0.0f;

    const float delta_height{max_height - min_height};

    for (uint32_t i{0}; i < iterations; i++) {
        const float iter_ratio = (float)i / (float)iterations;
        const float height = max_height - (iter_ratio * delta_height);

        TerraintPoints p1, p2;

        random_points(p1, p2);

        const int dir_X = p2.x - p1.x;
        const int dir_Z = p2.z - p1.z;

        for (uint32_t z{0}; z < map_size; z++) {
            for (uint32_t x{0}; x < map_size; x++) {
                int dir_Xin = x - p1.x;
                int dir_Zin = z - p1.z;

                const int cross_product = (dir_Xin * dir_Z) - (dir_X * dir_Zin);

                if (cross_product > 0) {
                    height_map_2D[(map_size * z) + x] += height;
                }
            }
        }
    }

    normalize(min_height, max_height);
    fill_map_terrain(min_height, max_height);
}

void FaultFormation::random_points(TerraintPoints& p1, TerraintPoints& p2) {
    p1.x = rand() % map_size;
    p1.z = rand() % map_size;

    int cnt{0};
    do {
        p2.x = rand() % map_size;
        p2.z = rand() % map_size;

        if (cnt++ >= 1000) {
            printf("Loop infinito...\n");
            throw 1;
        }
    } while (p1.is_equal(p2));
}

void FaultFormation::normalize(const float min_range, const float max_range) {
    float min_v{height_map_2D[0]}, max_v{height_map_2D[0]};

    for (uint32_t i{1}; i < map_size * map_size; i++) {
        if (height_map_2D[i] < min_v) {
            min_v = height_map_2D[i];
        }
        if (height_map_2D[i] > max_v) {
            max_v = height_map_2D[i];
        }
    }

    if (max_v <= min_v) return;

    const float min_max_delta = max_v - min_v;
    const float min_max_range = max_range - min_range;

    for (uint32_t i{0}; i < map_size * map_size; i++) {
        height_map_2D[i] = (((height_map_2D[i] - min_v) / min_max_delta) * min_max_range) + min_range;
    }
}