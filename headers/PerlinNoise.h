//
// Created by Cafer SELLI on 1.06.2024.
//

#ifndef HW3_PERLINNOISE_H
#define HW3_PERLINNOISE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

glm::vec2 randomGradient(int ix, int iy);

float dotGridGradient(int ix, int iy, float x, float y);

float interpolate(float a, float b, float t);

//float interpolate(float a0, float a1, float w)
//{
//    return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
//}

float perlin_noise(float x, float y);


#endif //HW3_PERLINNOISE_H
