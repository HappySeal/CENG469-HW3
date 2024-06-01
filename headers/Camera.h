//
// Created by Cafer SELLI on 2.05.2024.
//

#ifndef SAMPLEGL_CAMERA_H
#define SAMPLEGL_CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL 1

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "shaderUtils.h"
#include "../interfaces/IControllable.h"

// two different key bindings for arrow keys and wasd keys
enum KEY_BINDING {
    ARROW_KEYS,
    WASD_KEYS
};

// corresponding keys according to key binding
const int FORWARD_KEY[2] = {GLFW_KEY_UP, GLFW_KEY_I};
const int BACKWARD_KEY[2] = {GLFW_KEY_DOWN, GLFW_KEY_K};
const int LEFT_KEY[2] = {GLFW_KEY_LEFT, GLFW_KEY_J};
const int RIGHT_KEY[2] = {GLFW_KEY_RIGHT, GLFW_KEY_L};

const int UP_KEY[2] = {GLFW_KEY_PAGE_UP, GLFW_KEY_SPACE};
const int DOWN_KEY[2] = {GLFW_KEY_PAGE_DOWN, GLFW_KEY_LEFT_CONTROL};
const int LEFT_ROTATE_KEY[2] = {GLFW_KEY_LEFT, GLFW_KEY_U};
const int RIGHT_ROTATE_KEY[2] = {GLFW_KEY_RIGHT, GLFW_KEY_O};


class Camera: public IControllable {
public:
    glm::vec3 Position;
    glm::vec3 Orientation;
    glm::vec3 Up;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 cameraMatrix;

    bool firstClick = true;
    bool snapGround = true;

   int width;
   int height;

   float speed = 0.1f;
   float sensitivity = 100.0f;

    KEY_BINDING keyBinding = WASD_KEYS;

    Camera(int _width, int _height, glm::vec3 pos = glm::vec3(0,0,0), float _FOVdeg = 45.0f, float _nearPlane = 0.1f, float _farPlane = 100.0f);
    void updateMatrix();
    void Matrix(Shader& shader, const char* uniform);
    void HandleControl(GLFWwindow* window) override;
};

#endif //SAMPLEGL_CAMERA_H
