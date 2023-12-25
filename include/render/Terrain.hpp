#pragma once
class Terrain {
private:
    //=====>>> Variables Privadas
    #define TERRAIN_SIZE 800.0f
    #define VERTEX_COUNT 128
    float x, y;

public:
    //=====>>> Constructor y Destructor
    Terrain(int gridX, int gridY);
    ~Terrain();

    //=====>>> Funciones
    void generateTerrain();
};