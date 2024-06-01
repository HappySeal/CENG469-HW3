//
// Created by Cafer SELLI on 7.05.2024.
//

#ifndef SAMPLEGL_VERTEX_H
#define SAMPLEGL_VERTEX_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>


class Vertex{
public:
    glm::vec3 pos;
    Vertex(){
        pos = glm::vec3(0,0,0);
    }
    Vertex(float x, float y, float z) : pos(glm::vec3(x, y, z)) { }
    explicit Vertex(const glm::vec3& vIn): pos(vIn){}
};

#endif //SAMPLEGL_VERTEX_H
