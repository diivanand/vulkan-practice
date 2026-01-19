//
// Created by Diivanand Ramalingam on 1/19/26.
//

#include <stdexcept>

#include <GLFW/glfw3.h>

#include "glfw_initialization.h"

GlfwInitialization::GlfwInitialization() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

GlfwInitialization::~GlfwInitialization() {
    glfwTerminate();
}
