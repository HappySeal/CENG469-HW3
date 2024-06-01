//
// Created by Cafer SELLI on 11.04.2024.
//

#include "../headers/VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size){
    // Generate buffer with ID
    glGenBuffers(1, &ID);

    // Bind buffer as array buffer because we will store vertices in it
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind(){
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete(){
    glDeleteBuffers(1, &ID);
}
