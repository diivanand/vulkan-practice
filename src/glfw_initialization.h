//
// Created by Diivanand Ramalingam on 1/19/26.
//

#pragma once

/// @brief RAII wrapper for GLFW initialization and termination.
///
/// Initializes GLFW on construction and terminates on destruction.
/// Non-copyable to ensure single initialization lifetime.
class GlfwInitialization {
public:
    /// @brief Initializes the GLFW library.
    /// @throws std::runtime_error if GLFW initialization fails.
    GlfwInitialization();

    /// @brief Terminates the GLFW library and cleans up resources.
    ~GlfwInitialization();

    GlfwInitialization(const GlfwInitialization&) = delete;
    GlfwInitialization& operator=(const GlfwInitialization&) = delete;
};
