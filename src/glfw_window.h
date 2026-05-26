//
// Created by Diivanand Ramalingam on 1/19/26.
//

#pragma once

#include <memory>
#include <string_view>

#include <GLFW/glfw3.h>

namespace windowing {

/// @brief Custom deleter for GLFWwindow to enable RAII with unique_ptr.
struct GlfwWindowDeleter {
    /// @brief Destroys the GLFW window if non-null.
    /// @param window Pointer to the GLFWwindow to destroy.
    void operator()(GLFWwindow* window) const noexcept;
};

/// @brief Owning smart pointer type for GLFWwindow with automatic cleanup.
using UniqueGlfwWindow = std::unique_ptr<GLFWwindow, GlfwWindowDeleter>;

/// @brief Creates a GLFW window with the specified dimensions and title.
/// @param width Window width in pixels.
/// @param height Window height in pixels.
/// @param title Window title (UTF-8 encoded).
/// @return Unique pointer to the managed GLFWwindow.
/// @throws std::runtime_error if window creation fails.
auto createWindow(int width, int height, std::string_view title) -> UniqueGlfwWindow;

} // namespace windowing
