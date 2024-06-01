#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <map>
#include "../headers/Mesh.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))
#define RENDERING_TRIANGLES 3
#define RENDERING_QUADS 4

using namespace std;

int Mesh::VertexDataSize() const
{
    return this->vertices.size() * 3 * sizeof(GLfloat);
}

int Mesh::NormalDataSize() const
{
    return this->normals.size() * 3 * sizeof(GLfloat);
}

void Mesh::setupMesh()
{
    VAO = 0;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    // Allocate memory for vertex data
    GLfloat* vertexData = new GLfloat[vertices.size() * 3];
    GLfloat* normalData = new GLfloat[normals.size() * 3];

    // Copy vertex and normal data into the vertexData array
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        size_t offset = i * 3;
        vertexData[offset] = vertices[i].pos.x;
        vertexData[offset + 1] = vertices[i].pos.y;
        vertexData[offset + 2] = vertices[i].pos.z;
    }

    for (size_t i = 0; i < normals.size(); ++i)
    {
        size_t offset = i * 3;
        normalData[offset] = normals[i].normal.x;
        normalData[offset + 1] = normals[i].normal.y;
        normalData[offset + 2] = normals[i].normal.z;
    }

    std::cout << vertices.size() << " " << normals.size() << std::endl;
    // Upload vertex data to VBO


    points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), vertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    normals_vbo = 0;
    glGenBuffers(1, &normals_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), normalData, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // Free dynamically allocated memory
    delete[] vertexData;
    delete[] normalData;
}

void Mesh::Bind(){
    glBindVertexArray(VAO);
}

void Mesh::drawMesh() {
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void Mesh::calculateNormals()
{
    std::map<glm::vec3, glm::vec3*> normalMap;

    normals.clear();
    for (int i = 0; i < vertices.size(); i++)
    {
        Normal n = Normal(glm::vec3(0));
        normals.push_back(n);
    }

    if(renderingMode == RENDERING_QUADS)
    {
        for (int i = 0; i < vertices.size(); i+=6)
        {
            glm::vec3 n;
            glm::vec3 n1 = glm::vec3(0.0f), n2 = glm::vec3(0.0f);
            glm::vec3 v1(vertices[i].pos);
            glm::vec3 v2(vertices[i+1].pos);
            glm::vec3 v3(vertices[i+2].pos);

            glm::vec3 v4(vertices[i+3].pos);
            glm::vec3 v5(vertices[i+4].pos);
            glm::vec3 v6(vertices[i+5].pos);

            n1 = glm::cross((v2 - v1), (v3 - v1));
            n2 = glm::cross((v5 - v4), (v6 - v4));

            n = (n1+n2)/2.0f;
            for (size_t j = 0; j < 6; j++)
            {
                normals[i+j].normal = n;
            }
        }
    }
    else if(renderingMode == RENDERING_TRIANGLES)
    {
        for (int i = 0; i < vertices.size(); i+=3)
        {
            glm::vec3 n;
            glm::vec3 v1(vertices[i].pos);
            glm::vec3 v2(vertices[i+1].pos);
            glm::vec3 v3(vertices[i+2].pos);

            n = glm::cross((v2 - v1), (v3 - v1));

            for (size_t j = 0; j < 3; j++)
            {
                normals[i+j].normal = n;
            }
        }
    }

    for (int i = 0; i < normals.size(); i++)
    {
        auto t = glm::vec3(vertices[i].pos.x, vertices[i].pos.y, vertices[i].pos.z);
        glm::vec3 *n = new glm::vec3(normals[i].normal);
        normals[i].normal = glm::normalize(*n);
    }
}
