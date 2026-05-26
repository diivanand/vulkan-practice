//
// Created by Diivanand Ramalingam on 1/19/26.
//

#include "glfw_window.h"

#include <stdexcept>

namespace windowing {

void GlfwWindowDeleter::operator()(GLFWwindow* window) const noexcept {
    if (window) {
        glfwDestroyWindow(window);
    }
}

UniqueGlfwWindow createWindow(const int width, const int height, std::string_view title) {

    GLFWwindow* rawWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

    if (!rawWindow) {
        throw std::runtime_error("glfwCreateWindow failed");
    }

    return UniqueGlfwWindow{rawWindow};
}

} // namespace windowing
