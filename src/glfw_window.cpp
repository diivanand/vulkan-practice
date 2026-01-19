//
// Created by Diivanand Ramalingam on 1/19/26.
//

#include "glfw_window.h"
#include <stdexcept>

void GLFWWindowDeleter::operator()(GLFWwindow* w) const noexcept {
    if (w) {
        glfwDestroyWindow(w);
    }
}

unique_glfw_window create_window(
    const int width,
    const int height,
    std::string_view title) {

    GLFWwindow* raw =
        glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

    if (!raw) {
        throw std::runtime_error("glfwCreateWindow failed");
    }

    return unique_glfw_window{raw};
}
