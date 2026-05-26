#include <cstdlib>

#include <GLFW/glfw3.h>

#include "glfw_initialization.h"
#include "glfw_window.h"

auto main() -> int {
    windowing::GlfwInitialization glfw; // RAII init/terminate

    const windowing::UniqueGlfwWindow window = windowing::createWindow(800, 600, "Vulkan Engine");

    while (glfwWindowShouldClose(window.get()) == GLFW_FALSE) {
        glfwPollEvents();
    }

    // The window is destroyed before glfw, whose destructor terminates GLFW.
    return EXIT_SUCCESS;
}
