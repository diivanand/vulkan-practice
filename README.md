# vulkan-practice

A C++20 Vulkan learning project using GLFW for windowing and GLM for math. Dependencies (GLFW, GLM, GoogleTest) are fetched automatically via CMake FetchContent.

## Prerequisites

- CMake >= 3.22
- A C++20-capable compiler (Clang recommended — ASan and warnings are configured for Clang/GCC)
- [Vulkan SDK](https://vulkan.lunarg.com/) installed and `VULKAN_SDK` set in your environment
- Optional: `clang-tidy`, `cppcheck` (enabled by default; CMake silently skips either if not found)

## Build

```sh
cmake -B build
cmake --build build
```

To disable the static analysis tools or tests at configure time:

```sh
cmake -B build -DENABLE_CLANG_TIDY=OFF -DENABLE_CPPCHECK=OFF -DENABLE_TESTS=OFF
```

The main executable is produced at `build/vulkan_practice`.

## Run

```sh
./build/vulkan_practice
```

## Test

Build and run the full test suite:

```sh
cmake -B build
cmake --build build --target unit_tests
ctest --test-dir build --output-on-failure
```

### Run a specific test or filter by name

Use `--tests-regex` (or `-R`) to match by test name:

```sh
# Run only the Sanity suite
ctest --test-dir build -R "Sanity"

# Run only the MockExample suite
ctest --test-dir build -R "MockExample"
```

Alternatively, invoke the test binary directly and pass a GoogleTest filter:

```sh
# List all discovered tests
./build/tests/unit_tests --gtest_list_tests

# Run a single test case
./build/tests/unit_tests --gtest_filter="Sanity.Basic"

# Run an entire suite
./build/tests/unit_tests --gtest_filter="MockExample.*"
```

> Tests are built with AddressSanitizer enabled. Set `ASAN_OPTIONS=detect_leaks=1` before running if you want leak detection on top of memory error detection.

## Shaders

GLSL shaders live in `assets/shaders/`. Compile them to SPIR-V with `glslc` (included in the Vulkan SDK):

```sh
glslc assets/shaders/triangle.vert -o assets/shaders/triangle.vert.spv
glslc assets/shaders/triangle.frag -o assets/shaders/triangle.frag.spv
```
