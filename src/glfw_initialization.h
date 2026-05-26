//
// Created by Diivanand Ramalingam on 1/19/26.
//

#pragma once

namespace windowing {

/// @brief RAII wrapper for GLFW initialization and termination.
///
/// Initializes GLFW on construction and terminates on destruction.
/// Non-copyable and non-movable to keep termination tied to the object that
/// successfully initialized GLFW.
class GlfwInitialization {
public:
    /// @brief Initializes the GLFW library.
    /// @throws std::runtime_error if GLFW initialization fails.
    GlfwInitialization();

    /// @brief Terminates the GLFW library and cleans up resources.
    ~GlfwInitialization() noexcept;

    GlfwInitialization(const GlfwInitialization&) = delete;
    GlfwInitialization& operator=(const GlfwInitialization&) = delete;
    GlfwInitialization(GlfwInitialization&&) = delete;
    GlfwInitialization& operator=(GlfwInitialization&&) = delete;
};

} // namespace windowing
