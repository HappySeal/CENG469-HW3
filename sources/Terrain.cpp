//
// Created by Cafer SELLI on 1.06.2024.
//

#include "../headers/Terrain.h"
#include <iostream>

Terrain::Terrain(int width, int height) : width(width), height(height) {
    NUM_STRIPS = height - 1;
    VERTICES_PER_STRIP = width * 2;
    generateTerrain();
}

void Terrain::generateTerrain() {
    float freq = 1, amp = 1;
    float yScale = 16.0f, yShift = 16.0f;  // apply a scale+shift to the height data
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
            // retrieve texel for (i,j) tex coord
            // raw height at coordinate
            freq = 1;
            amp = 1;
            float y = 0;
            for(int k = 0; k < 8; k++) {
                y += perlin_noise(i * freq / GRID_SIZE, j * freq / GRID_SIZE) * amp;
                freq *= 2;
                amp *= 0.5;
            }

            if(y < -1) y = -1;
            if(y > 1) y = 1;

            y = y * yScale + yShift; // apply scale and shift

            // vertex
            vertices.push_back( -height/2.0f + i );        // v.x
            vertices.push_back(y); // v.y
            vertices.push_back( -width/2.0f + j );        // v.z
        }
    }

    // index generation
    for(unsigned int i = 0; i < height-1; i++)       // for each row a.k.a. each strip
    {
        for(unsigned int j = 0; j < width; j++)      // for each column
        {
            for(unsigned int k = 0; k < 2; k++)      // for each side of the strip
            {
                indices.push_back(j + width * (i + k));
            }
        }
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),       // size of vertices buffer
                 &vertices[0],                          // pointer to first element
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int), // size of indices buffer
                 &indices[0],                           // pointer to first element
                 GL_STATIC_DRAW);
}

float Terrain::getHeightAt(const glm::vec2& pos) const{
    if(pos.x < -width/2 || pos.x > width/2 || pos.y < -height/2 || pos.y > height/2)
        return 0;
    int i = (int)(pos.x + height/2);
    int j = (int)(pos.y + width/2);

    float y00 = vertices[(i * width + j)*3 + 1];
    float y01 = vertices[(i * width + j + 1)*3 + 1];
    float y10 = vertices[((i + 1) * width + j)*3 + 1];
    float y11 = vertices[((i + 1) * width + j + 1)*3 + 1];

    float x = (pos.x + height/2 - i);
    float y = (pos.y + width/2 - j);

    float y0 = y00 * (1 - x) + y01 * x;
    float y1 = y10 * (1 - x) + y11 * x;

    return y0 * (1 - y) + y1 * y;
}

void Terrain::draw() {
    glBindVertexArray(VAO);
    // render the mesh triangle strip by triangle strip - each row at a time
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
    {
        glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
                       VERTICES_PER_STRIP, // number of indices to render
                       GL_UNSIGNED_INT,     // index data type
                       (void*)(sizeof(unsigned int)
                               * VERTICES_PER_STRIP
                               * strip)); // offset to starting index
    }
}