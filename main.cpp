#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "headers/shaderUtils.h"
#include "headers/Camera.h"
#include "headers/Cubemap.h"
#include "headers/Terrain.h"

#define WIDTH 800
#define HEIGHT 600
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Terrain* terrain;
Camera* camera;
Cubemap* skybox;

float grass_height = 0.6f;
float grass_hue = 0.0f;
float wind_speed = 3.0f;

const float max_grass_height = 2.0f;
const float min_grass_height = 0.2;


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
    terrain = new Terrain(8,8);

    auto model = new Model("./resources/models/bunny.obj", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(1.0f));


    auto *defaultShader = new Shader("./resources/shaders/terrain.vert", "./resources/shaders/terrain.frag");
    auto *grassShader = new Shader("./resources/shaders/vert.glsl", "./resources/shaders/frag.glsl","./resources/shaders/geom.glsl");

    skybox->width = WIDTH;
    skybox->height = HEIGHT;

   // grassShader->Set

    assert(glGetError() == GL_NO_ERROR);


    auto height = terrain->getHeightAt(glm::vec2(camera->Position.x, camera->Position.z));
    if(height != -1.0){
        camera->Position.y = height + camera->eye_height;
    }
    camera->updateMatrix();


    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Clear the color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera->setHeight(terrain->getHeightAt(glm::vec2(camera->Position.x, camera->Position.z)));

        camera->updateMatrix();
        camera->HandleControl(window);
        //skybox->HandleControl(window);

        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
            grass_hue += 0.1f;
            if(grass_hue > 1.0f)
                grass_hue = 0.0f;
        }

        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
            grass_hue -= 0.1f;
            if(grass_hue < 0.0f)
                grass_hue = 1.0f;
        }


        defaultShader->Activate();
        camera->Matrix(*defaultShader, "camMatrix");
        defaultShader->SetMat4("model", new glm::mat4(1.0f));
        terrain->draw();



        grassShader->Activate();
//        model->bindMesh();
        camera->Matrix(*grassShader, "camMatrix");
        grassShader->SetMat4("model", new glm::mat4(1.0f));
        grassShader->SetFloat("uTime", glfwGetTime());
        grassShader->SetFloat("uHeight", grass_height);
        grassShader->SetFloat("uHue", grass_hue);
        grassShader->SetFloat("uWindSpeed",wind_speed);
//        model->drawMesh();
        grassShader->SetInt("uWindMapX1",0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, terrain->windMapX1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, terrain->windMapX2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, terrain->windMapX4);

        terrain->draw();
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
    //skybox->OnKeyInput(key, action);

    if(key == GLFW_KEY_W && action == GLFW_PRESS){
        grass_height += 0.1f;
        if(grass_height > max_grass_height)
            grass_height = max_grass_height;
    }

    if(key == GLFW_KEY_S && action == GLFW_PRESS){
        grass_height -= 0.1f;
        if(grass_height < min_grass_height)
            grass_height = min_grass_height;
    }

    if(key == GLFW_KEY_D && action == GLFW_PRESS){
        wind_speed += 0.1f;
    }

    if(key == GLFW_KEY_A && action == GLFW_PRESS){
        wind_speed -= 0.1f;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}