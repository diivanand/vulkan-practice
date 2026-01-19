set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

cmake_minimum_required(VERSION 3.22)
project(VulkanPractice LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_TESTS "Build unit tests" ON)
option(ENABLE_CLANG_TIDY "Enable clang-tidy during builds" ON)
option(ENABLE_CPPCHECK "Enable cppcheck during builds" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(tools)
include(sanitizers)

find_package(Vulkan REQUIRED)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.9
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

add_executable(vulkan_practice
  src/main.cpp
  src/vk_app.h
  src/vk_app.cpp
)

target_link_libraries(vulkan_practice
  PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
)

target_compile_definitions(vulkan_practice PRIVATE GLFW_INCLUDE_NONE)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
  target_compile_options(vulkan_practice PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()