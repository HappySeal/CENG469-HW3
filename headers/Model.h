//
// Created by Cafer SELLI on 7.05.2024.
//

#ifndef SAMPLEGL_MODEL_H
#define SAMPLEGL_MODEL_H

#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glew.h>
//#include <OpenGL/gl3.h>   // The GL Header File
#include <GLFW/glfw3.h> // The GLFW header
#include <glm/glm.hpp> // GL Math library header
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Mesh.h"
#include "Parser.h"
#include <iostream>
class Model
{
public:

    std::vector<Mesh* > meshes;
    int index;
    glm::vec3 position;
    glm::vec3 angleInDegrees;
    glm::vec3 scale;
    Color color;

    int renderMode = 0;

    bool isSubdivision = false;
    Model(const char* meshFileName, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec4 clr)
    {
        Mesh* initialMesh = new Mesh();
        if(!Parser::ParseObj(meshFileName, initialMesh)){
            std::cout << "Error parsing obj file" << std::endl;
        }
        std::cout << "Loading model: " << meshFileName << std::endl;
        meshes.push_back(initialMesh);
        this->position = position;
        this->angleInDegrees = rotation;
        this->scale = scale;
        index = 0;
        color = Color(clr);
        meshes[0]->setupMesh();

    }

    glm::mat4 TranslationMatrix() const
    {
        return glm::translate(glm::mat4(1.0), position);
    }
    glm::mat4 RotationMatrix() const
    {
        glm::mat4 res(1.0);
        res = glm::rotate<float>(res, (angleInDegrees.x / 180.) * M_PI, glm::vec3(1.0, 0.0, 0.0));
        res = glm::rotate<float>(res, (angleInDegrees.y / 180.) * M_PI, glm::vec3(0.0, 1.0, 0.0));
        res = glm::rotate<float>(res, (angleInDegrees.z / 180.) * M_PI, glm::vec3(0.0, 0.0, 1.0));
        return res;
    }

    glm::mat4 ScaleMatrix() const
    {
        return glm::scale(glm::mat4(1.0f), scale);
    }

    glm::mat4 ModelingMatrix()
    {
        return TranslationMatrix() * RotationMatrix() * ScaleMatrix();
    }

    void bindMesh(){
        meshes[index]->Bind();
    }

    void drawMesh(){
        meshes[index]->drawMesh();
    };

    ~Model(){
        for(int i = 0; i < meshes.size(); i++){
            delete meshes[i];
        }
    }
};

#endif //SAMPLEGL_MODEL_H
