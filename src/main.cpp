#include <cstdint>

#include <GLFW/glfw3.h>

#include "glfw_initialization.h"
#include "glfw_window.h"

int32_t main(int32_t, char**) {
    GlfwInitialization glfw; // RAII init/terminate

    unique_glfw_window window = create_window(800, 600, "Vulkan Engine");

    while (!glfwWindowShouldClose(window.get())) {
        glfwPollEvents();
    }

    // The window is destroyed before glfw, whose destructor terminates GLFW.
    return 0;
}
