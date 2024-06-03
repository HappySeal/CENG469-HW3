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

void checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath, const std::string &geometryShaderPath, const std::string &tessControlShaderPath, const std::string &tessEvalShaderPath) {
    std::string vertexShaderSource = get_file_content(vertexShaderPath);
    std::string fragmentShaderSource = get_file_content(fragmentShaderPath);
    std::string geometryShaderSource = get_file_content(geometryShaderPath);
    std::string tessControlShaderSource = get_file_content(tessControlShaderPath);
    std::string tessEvalShaderSource = get_file_content(tessEvalShaderPath);

    const GLchar *vertexShaderSourceC = (GLchar *) vertexShaderSource.c_str();
    const GLchar *fragmentShaderSourceC = (GLchar *) fragmentShaderSource.c_str();
    const GLchar *geometryShaderSourceC = (GLchar *) geometryShaderSource.c_str();
    const GLchar *tessControlShaderSourceC = (GLchar *) tessControlShaderSource.c_str();
    const GLchar *tessEvalShaderSourceC = (GLchar *) tessEvalShaderSource.c_str();

    std::cout << "Compiling vertex shader" << vertexShaderPath << std::endl;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceC, NULL);
    glCompileShader(vertexShader);
    assert(glGetError() == GL_NO_ERROR);


    std::cout << "Compiling fragment shader" << fragmentShaderPath << std::endl;
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, NULL);
    glCompileShader(fragmentShader);
    assert(glGetError() == GL_NO_ERROR);


    std::cout << "Compiling tess control shader" << tessControlShaderPath << std::endl;
    GLuint tesControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tesControlShader, 1, &tessControlShaderSourceC, NULL);
    glCompileShader(tesControlShader);
    assert(glGetError() == GL_NO_ERROR);


    std::cout << "Compiling tess eval shader" << tessEvalShaderPath << std::endl;
    GLuint tesEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tesEvalShader, 1, &tessEvalShaderSourceC, NULL);
    glCompileShader(tesEvalShader);
    assert(glGetError() == GL_NO_ERROR);


//    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
//    glShaderSource(geometryShader, 1, &geometryShaderSourceC, NULL);
//    glCompileShader(geometryShader);
//    assert(glGetError() == GL_NO_ERROR);
//

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glAttachShader(ID, tesControlShader);
    glAttachShader(ID, tesEvalShader);
//    glAttachShader(ID, geometryShader);
    glLinkProgram(ID);

    checkCompileErrors();

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(tesControlShader);
    glDeleteShader(tesEvalShader);
}

Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath,
               const std::string &geometryShaderPath){
    std::string vertexShaderSource = get_file_content(vertexShaderPath);
    std::string fragmentShaderSource = get_file_content(fragmentShaderPath);
    std::string geometryShaderSource = get_file_content(geometryShaderPath);

    const GLchar * vertexShaderSourceC = (GLchar *) vertexShaderSource.c_str();
    const GLchar* fragmentShaderSourceC = (GLchar *) fragmentShaderSource.c_str();
    const GLchar * geometryShaderSourceC = (GLchar *) geometryShaderSource.c_str();

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



    std::cout << "Compiling geometry shader" << geometryShaderPath << std::endl;
    // Create Geometry Shader
    assert(glGetError() == GL_NO_ERROR);
    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

    glShaderSource(geometryShader, 1, &geometryShaderSourceC, NULL);
    assert(glGetError() == GL_NO_ERROR);

    glCompileShader(geometryShader);

    assert(glGetError() == GL_NO_ERROR);



    // Attach Vertex and Fragment Shaders to Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glAttachShader(ID, geometryShader);
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
    glDeleteShader(geometryShader);
}

void Shader::checkCompileErrors() {
    GLint status;
    glGetProgramiv(ID, GL_LINK_STATUS, &status);

    if (status != GL_TRUE)
    {
        GLint logLength;
        glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetShaderInfoLog(ID, logLength, &logLength, log.data());
        std::cerr << "Shader compilation failed: " << log.data() << std::endl;
        exit(-1);
    }
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

void Shader::SetFloat2(const int location, const float value1, const float value2) const {
    glProgramUniform2f(ID, location, value1, value2);
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