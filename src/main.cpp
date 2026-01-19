#include "glfw_initialization.h"
#include "glfw_window.h"
#include "vk_app.h"

#include "../cmake-build-debug/_deps/glfw-src/include/GLFW/glfw3.h"

int32_t main(int32_t, char**) {
    GlfwInitialization glfw; // RAII init/terminate
    VkApp app;
    app.run();

    std::unique_ptr<GLFWwindow, GLFWWindowDeleter> window = create_window(800, 600, "Vulkan Engine");

    while (!glfwWindowShouldClose(window.get())) {
        glfwPollEvents();
    }

    // window destroyed automatically since unique_ptr goes out of scope and calls GLFWWindowDeleter
    // which in turn calls glfwTerminate
    return 0;
}
