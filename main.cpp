#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "headers/shaderUtils.h"
#include "headers/Camera.h"
#include "headers/Model.h"
#include "headers/Cubemap.h"

#define WIDTH 800
#define HEIGHT 600
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Camera* camera;
Cubemap* skybox;

int main(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Notating we are using OpenGL 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Notating we are using OpenGL 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Notating we are using OpenGL Core Profile

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "HW3", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    camera = new Camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f), 90.0f, 0.1f, 100.0f);
    skybox = new Cubemap("./resources/hdr/Grassland.hdr", camera->projectionMatrix);

    skybox->width = WIDTH;
    skybox->height = HEIGHT;
    // Shader


//    skybox->Bind();
//    skybox->skybox->SetMat4("projection", &camera->projectionMatrix);

    assert(glGetError() == GL_NO_ERROR);

    // Enable depth testing

    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
//
//    std::cout << "Light count " << (int)(medianCut->lightPointsByIter[medianCut->iterationCount]).size() << std::endl;
//    for (int i = 0; i < medianCut->lightPointsByIter[medianCut->iterationCount].size(); ++i) {
//        std::cout << "Light point " << i << ": " << medianCut->lightPointsByIter[medianCut->iterationCount][i].pos.x << " " << medianCut->lightPointsByIter[medianCut->iterationCount][i].pos.y << " " << medianCut->lightPointsByIter[medianCut->iterationCount][i].pos.z << " " << medianCut->lightPointsByIter[medianCut->iterationCount][i].color.r << " " << medianCut->lightPointsByIter[medianCut->iterationCount][i].color.g << " " << medianCut->lightPointsByIter[medianCut->iterationCount][i].color.b << std::endl;
//    }


    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Clear the color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera->updateMatrix();
        camera->HandleControl(window);
        skybox->HandleControl(window);

        skybox->Bind();

        skybox->Draw(camera->viewMatrix);
        assert(glGetError() == GL_NO_ERROR);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    skybox->OnKeyInput(key, action);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}