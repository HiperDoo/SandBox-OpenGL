#ifndef RENDERER_H
#define RENDERER_H

#include "Resources.hpp"
#include "Camera.hpp"

extern Camera camera;

extern void renderSkyBox();

extern void renderCppImg();

extern void renderPiramid(const float rotation);

extern void renderFloor();

extern void renderLight();

#endif