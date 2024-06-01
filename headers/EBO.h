//
// Created by Cafer SELLI on 11.04.2024.
//

#ifndef SAMPLEGL_EBO_H
#define SAMPLEGL_EBO_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class EBO{
public:
    GLuint ID;
    EBO(GLuint* indices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};

#endif //SAMPLEGL_EBO_H
