//
// Created by Cafer SELLI on 7.05.2024.
//

#include "../headers/Cubemap.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../headers/stb_image.h"
#endif


Cubemap::Cubemap(const std::string& texturePath, glm::mat4 &projection) {
    int skyboxRes = 2048;

    model = new Model("./resources/models/cube.obj", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(1.0f));

    skybox = new Shader("./resources/shaders/skybox.vert", "./resources/shaders/skybox.frag");
    equirectangularToCubemap = new Shader("./resources/shaders/eqrt2cm.vert", "./resources/shaders/eqrt2cm.frag");
    skyboxModelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

    exposure = 0.18f;

    skybox->Activate();
    skybox->SetInt("environmentMap", 0);

    glGenFramebuffers(1, &captureFBO);

    glGenRenderbuffers(1, &captureRBO);


    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, skyboxRes, skyboxRes);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


    stbi_set_flip_vertically_on_load(true);
    int _width, _height, nrComponents;
    float *data = stbi_loadf(texturePath.c_str(), &_width, &_height, &nrComponents, 0);
    //cout first couple of data

    unsigned int hdrTexture;
    if (data)
    {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load HDR image." << std::endl;
    }



    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, skyboxRes, skyboxRes, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    assert(glGetError() == GL_NO_ERROR);


    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    glm::mat4 captureViews[] =
            {
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
            };

    equirectangularToCubemap->Activate();
    equirectangularToCubemap->SetInt("equirectangularMap", 0);
    equirectangularToCubemap->SetMat4("projection", &captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    assert(glGetError() == GL_NO_ERROR);


    glViewport(0, 0, skyboxRes, skyboxRes); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        equirectangularToCubemap->SetMat4("view", &captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model->bindMesh();
        model->drawMesh();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    skybox->Activate();
    skybox->SetMat4("projection", &projection);
    skybox->SetFloat("exposure",exposure);

    assert(glGetError() == GL_NO_ERROR);
}

void Cubemap::Bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
}

void Cubemap::BindIrradiance(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
}

void Cubemap::Unbind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::Delete() {
    glDeleteTextures(1, &ID);
}

void Cubemap::Draw(glm::mat4 &view) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    skybox->Activate();
    skybox->SetMat4("view", &view);
    skybox->SetMat4("model", &skyboxModelMatrix);
    skybox->SetFloat("exposure",exposure);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    model->bindMesh();
    model->drawMesh();
}

void Cubemap::OnKeyInput(int key, int action) {
    if(key == GLFW_KEY_W && action == GLFW_RELEASE){
        exposure *= 2.f;
    }

    if(key == GLFW_KEY_S && action == GLFW_RELEASE){
        exposure /= 2.f;
    }
}

void Cubemap::HandleControl(GLFWwindow *window) {

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        angle += 1.0f;
        skyboxModelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        angle -= 1.0f;
        skyboxModelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    }
}

// Path: sources/Model.cpp
