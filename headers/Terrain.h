//
// Created by Cafer SELLI on 1.06.2024.
//

#ifndef HW3_TERRAIN_H
#define HW3_TERRAIN_H

#include <vector>
#include "PerlinNoise.h"

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Terrain{
public:
    int width, height;
    static const int GRID_SIZE = 64;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int NUM_STRIPS;
    unsigned int VERTICES_PER_STRIP;

    GLuint VAO, VBO, EBO;

    Terrain(int width, int height);
    float getHeightAt(const glm::vec2& pos) const;
    void generateTerrain();
    void draw();
};

#endif //HW3_TERRAIN_H
