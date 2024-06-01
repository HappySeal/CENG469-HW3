//
// Created by Cafer SELLI on 11.04.2024.
//

#include "../headers/EBO.h"

EBO::EBO(GLuint* indices, GLsizeiptr size){
    glGenBuffers(1, &ID);
    // Bind buffer as element array buffer because we will store indices in it
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::Bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete(){
    glDeleteBuffers(1, &ID);
}