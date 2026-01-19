//
// Created by Diivanand Ramalingam on 1/19/26.
//

#include "glfw_initialization.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

GlfwInitialization::GlfwInitialization() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

GlfwInitialization::~GlfwInitialization() {
    glfwTerminate();
}
