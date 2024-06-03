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
#include <iostream>

class Terrain{
public:
    int width, height;
    static const int GRID_SIZE = 64;
    float resolution = 1;
    GLuint screen;

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> indices;

    unsigned int NUM_STRIPS;
    unsigned int VERTICES_PER_STRIP;

    int divisions = 4;

    GLuint VAO, VBO, EBO, nVBO;

    Terrain(int width, int height);
    float getHeightAt(const glm::vec2& pos) const;
    void generateTerrainVertices();
    void generateTerrainIndices();
    void generateTerrain();
    void draw();
};

#endif //HW3_TERRAIN_H
