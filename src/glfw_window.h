//
// Created by Diivanand Ramalingam on 1/19/26.
//

#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <string_view>

// Deleter
struct GLFWWindowDeleter {
    void operator()(GLFWwindow* w) const noexcept;
};

// Owning window type
using unique_glfw_window =
    std::unique_ptr<GLFWwindow, GLFWWindowDeleter>;

// Factory
unique_glfw_window create_window(
    int width,
    int height,
    std::string_view title);


