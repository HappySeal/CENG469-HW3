//
// Created by Cafer SELLI on 7.05.2024.
//

#ifndef SAMPLEGL_MESH_H
#define SAMPLEGL_MESH_H

#include <vector>
#include "Vertex.h"
#include "Normal.h"
#include "Color.h"

class Mesh
{
public:
    int renderingMode = 3;
    int renderMode = 0;
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;

    int VertexDataSize() const;
    int NormalDataSize() const;
    int IndexDataSize();

    unsigned int VAO, points_vbo, normals_vbo, color_vbo;
    // void setupMeshIndexed();
    // void drawMeshIndexed();
    void Bind();
    void setupMesh();
    void drawMesh();
    void calculateNormals();
};

#endif //SAMPLEGL_MESH_H
