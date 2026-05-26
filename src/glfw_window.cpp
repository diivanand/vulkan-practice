//
// Created by Diivanand Ramalingam on 1/19/26.
//

#include "glfw_window.h"

#include <stdexcept>
#include <string>

namespace windowing {

void GlfwWindowDeleter::operator()(GLFWwindow* window) const noexcept {
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
}

auto createWindow(const int width, const int height, std::string_view title) -> UniqueGlfwWindow {
    const std::string windowTitle{title};
    GLFWwindow* rawWindow = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);

    if (rawWindow == nullptr) {
        throw std::runtime_error("glfwCreateWindow failed");
    }

    return UniqueGlfwWindow{rawWindow};
}

} // namespace windowing
