//
// Created by Cafer SELLI on 2.05.2024.
//

#include "../headers/Camera.h"

Camera::Camera(int _width, int _height, glm::vec3 pos, float _FOVdeg, float _nearPlane, float _farPlane){
    Position = pos;
    Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraMatrix = glm::mat4(1.0f);
    width = _width;
    height = _height;
    projectionMatrix = glm::perspective(glm::radians(_FOVdeg), (float)width / (float)height, _nearPlane, _farPlane);
}

void Camera::updateMatrix(){
    viewMatrix = glm::lookAt(Position, Position + Orientation, Up);

    cameraMatrix = projectionMatrix * viewMatrix;
}

void Camera::Matrix(Shader &shader, const char *uniform) {
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::HandleControl(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
        snapGround = !snapGround;
    }

    if(glfwGetKey(window, FORWARD_KEY[this->keyBinding]) == GLFW_PRESS){
        Position += speed * Orientation;
    }
    if(glfwGetKey(window, BACKWARD_KEY[this->keyBinding]) == GLFW_PRESS){
        Position -= speed * Orientation;
    }
    if(glfwGetKey(window, LEFT_KEY[this->keyBinding]) == GLFW_PRESS){
        Position -= glm::normalize(glm::cross(Orientation, Up)) * speed;
    }
    if(glfwGetKey(window, RIGHT_KEY[this->keyBinding]) == GLFW_PRESS){
        Position += glm::normalize(glm::cross(Orientation, Up)) * speed;
    }

    if(glfwGetKey(window, UP_KEY[this->keyBinding]) == GLFW_PRESS){
        Position += Up * speed;
    }
    if(glfwGetKey(window, DOWN_KEY[this->keyBinding]) == GLFW_PRESS){
        Position -= Up * speed;
    }

    if(glfwGetKey(window, LEFT_ROTATE_KEY[this->keyBinding]) == GLFW_PRESS){
        Orientation = glm::rotate(Orientation, glm::radians(-speed * sensitivity / 10), Up);
    }

    if(glfwGetKey(window, RIGHT_ROTATE_KEY[this->keyBinding]) == GLFW_PRESS){
        Orientation = glm::rotate(Orientation, glm::radians(speed * sensitivity / 10), Up);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        speed = 0.5f;
    }else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE){
        speed = 0.1f;
    }

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if(firstClick){
            glfwSetCursorPos(window, width/2, height/2);
            firstClick = false;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = sensitivity * (float)(mouseY - height/2) / height;
        float rotY = sensitivity * (float)(mouseX - width/2) / width;

        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        if(!(glm::angle(newOrientation, Up) <= glm::radians(5.0f) || glm::angle(newOrientation, -Up) <= glm::radians(5.0f))){
            Orientation = newOrientation;
        }

        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        glfwSetCursorPos(window, width/2, height/2);

    }else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}