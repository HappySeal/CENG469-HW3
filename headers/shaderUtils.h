//
// Created by Cafer SELLI on 11.04.2024.
//

#ifndef SAMPLEGL_SHADERUTILS_H
#define SAMPLEGL_SHADERUTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

std::string get_file_content(const std::string& filePath);

class Shader{
public:
    GLuint ID;
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath);
    Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath, const std::string &geometryShaderPath, const std::string &tessControlShaderPath, const std::string &tessEvalShaderPath);
    void checkCompileErrors();

    void Activate();
    void Delete();
    void SetMat4(const std::string& name, const glm::mat4* value) const;
    void SetVec4f(const std::string &name, const glm::vec4 *vec) const;
    void SetVec3f(const std::string &name, const glm::vec3 *vec) const;
    void SetFloat(const std::string &name, const float value) const;
    void SetFloat2(const int location, const float value1, const float value2) const;
    void SetCubeMap(const std::string &name, const GLuint texture) const;
    void SetInt(const std::string &name, const int value) const;
};


#endif //SAMPLEGL_SHADERUTILS_H
