//
// Created by Cafer SELLI on 11.05.2024.
//

#ifndef SAMPLEGL_ICONTROLLABLE_H
#define SAMPLEGL_ICONTROLLABLE_H

#include "GLFW/glfw3.h"

class IControllable {
public:
    virtual void HandleControl(GLFWwindow *window) = 0;
};

#endif //SAMPLEGL_ICONTROLLABLE_H
