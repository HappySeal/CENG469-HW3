//
// Created by Cafer SELLI on 7.05.2024.
//

#ifndef SAMPLEGL_COLOR_H
#define SAMPLEGL_COLOR_H

#include <GL/glew.h>

class Color{
public:
    explicit Color(glm::vec4 inCol) {
        r = inCol.x;
        g = inCol.y;
        b = inCol.z;
        a = inCol.w;
    }

    Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) : r(r), g(g), b(b), a(a) {}
    Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}

    GLfloat r, g, b, a;

    const glm::vec4 *toVec4f() {
        return new glm::vec4(r, g, b, a);
    }
};

#endif //SAMPLEGL_COLOR_H
