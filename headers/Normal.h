//
// Created by Cafer SELLI on 7.05.2024.
//

#ifndef SAMPLEGL_NORMAL_H
#define SAMPLEGL_NORMAL_H

#include <glm/vec3.hpp>
#include "GL/glew.h"
#include "glm/glm.hpp"

class Normal
{
public:
    Normal(float x, float y, float z) : normal(glm::vec3(x, y, z)){ }
    explicit Normal(const glm::vec3& vIn): normal(glm::vec3(vIn)){}
    Normal(){}
    glm::vec3 normal;
    void normalize(){
        normal = glm::normalize(normal);
    }
};
#endif //SAMPLEGL_NORMAL_H
