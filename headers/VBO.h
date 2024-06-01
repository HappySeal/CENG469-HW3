//
// Created by Cafer SELLI on 11.04.2024.
//

#ifndef SAMPLEGL_VBO_H
#define SAMPLEGL_VBO_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class VBO{
public:
    GLuint ID;
    VBO(GLfloat* vertices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};

#endif //SAMPLEGL_VBO_H
