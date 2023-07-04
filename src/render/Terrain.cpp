#include "Terrain.hpp"

//=====>>> Constructor y Destructor
Terrain::Terrain(int gridX, int gridY) :
    x(gridX * TERRAIN_SIZE), y(gridY * TERRAIN_SIZE) {}

Terrain::~Terrain() {}

//=====>>> Funciones
void Terrain::generateTerrain() {
    //int count = VERTEX_COUNT * VERTEX_COUNT;
}