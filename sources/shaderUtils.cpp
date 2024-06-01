//
// Created by Cafer SELLI on 11.04.2024.
//

#include "../headers/shaderUtils.h"

std::string get_file_content(const std::string& filePath){
    std::ifstream fileStream(filePath);
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    if(buffer.str().empty()){
        std::cerr << "Error: File " << filePath << " not found or empty" << std::endl;
        exit(1);
    }
    return buffer.str();
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath){
    std::string vertexShaderSource = get_file_content(vertexShaderPath);
    std::string fragmentShaderSource = get_file_content(fragmentShaderPath);

    const GLchar * vertexShaderSourceC = (GLchar *) vertexShaderSource.c_str();
    const GLchar* fragmentShaderSourceC = (GLchar *) fragmentShaderSource.c_str();

    ID = glCreateProgram();
    // Create Vertex Shader
    std::cout << "Compiling vertex shader" << vertexShaderPath <<std::endl;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    assert(glGetError() == GL_NO_ERROR);

    glShaderSource(vertexShader, 1, &vertexShaderSourceC, NULL);
    // Compile Vertex Shader
    assert(glGetError() == GL_NO_ERROR);

    glCompileShader(vertexShader);

    assert(glGetError() == GL_NO_ERROR);


    std::cout << "Compiling fragment shader" << fragmentShaderPath <<std::endl;
    // Create Fragment Shader
    assert(glGetError() == GL_NO_ERROR);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, NULL);
    // Compile Fragment Shader
    assert(glGetError() == GL_NO_ERROR);

    glCompileShader(fragmentShader);

    assert(glGetError() == GL_NO_ERROR);



    // Attach Vertex and Fragment Shaders to Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Link Shader Program
    glLinkProgram(ID);

    assert(glGetError() == GL_NO_ERROR);

    GLint status;
    glGetProgramiv(ID, GL_LINK_STATUS, &status);

    if (status != GL_TRUE)
    {
        std::cout << "Program link failed: " << status << std::endl;
        exit(-1);
    }
    // Delete Shaders (for getting rid of intermediate files)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate(){
    glUseProgram(ID);
}

void Shader::Delete(){
    glDeleteProgram(ID);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 *value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(*value));
}

void Shader::SetVec4f(const std::string &name, const glm::vec4 *vec) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), vec->x, vec->y, vec->z, vec->w);
}

void Shader::SetVec3f(const std::string &name, const glm::vec3 *vec) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vec->x, vec->y, vec->z);
}

void Shader::SetInt(const std::string &name, const int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, const float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetCubeMap(const std::string &name, const GLuint texture) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}