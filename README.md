# vulkan-practice

A C++20 Vulkan learning project using GLFW for windowing and GLM for math. The
current executable initializes GLFW, opens a window, and polls events until the
window closes. Vulkan is linked in preparation for the next rendering steps.

GLFW, GLM, and GoogleTest are downloaded and built by CMake with
`FetchContent`; do not install GLFW separately for this project.

## Prerequisites

- CMake 3.22 or newer.
- A C++20-capable compiler. AppleClang from Xcode is appropriate on macOS.
- A Vulkan SDK or Vulkan installation visible to CMake.
- `glslc` if you want to compile the shader sources.
- Optional: `clang-tidy` and `cppcheck`. CMake uses either when found and
  otherwise continues without it.

On macOS, the LunarG Vulkan SDK can be checked with:

```sh
command -v glslc
command -v vulkaninfo
```

The project works when CMake can locate Vulkan even if `VULKAN_SDK` is not set.
To check what CMake found after configuring:

```sh
grep -E 'Vulkan_(INCLUDE_DIR|LIBRARY|GLSLC_EXECUTABLE)' build/CMakeCache.txt
```

## Configure And Build

Run these commands from the repository root:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target vulkan_practice
```

The initial configure downloads GLFW, GLM, and GoogleTest, so it requires
network access. The executable is generated at:

```sh
./build/vulkan_practice
```

To build all configured targets, including unit tests:

```sh
cmake --build build
```

Useful configure variations:

```sh
# Skip static-analysis tools during builds.
cmake -S . -B build-fast -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_CLANG_TIDY=OFF -DENABLE_CPPCHECK=OFF
cmake --build build-fast

# Build only the application and omit the unit-test target.
cmake -S . -B build-app -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=OFF
cmake --build build-app --target vulkan_practice
```

## Run The Application

```sh
cmake --build build --target vulkan_practice
./build/vulkan_practice
```

The application opens a desktop window and therefore needs a graphical login
session. It is not currently a headless render test.

## Run Tests

The unit tests cover RAII and custom-deleter behavior. They do not create a GLFW
window or exercise Vulkan.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target unit_tests
ctest --test-dir build --output-on-failure
```

List the tests registered with CTest:

```sh
ctest --test-dir build -N
```

Run one suite through CTest:

```sh
ctest --test-dir build -R '^TypeTraits\.' --output-on-failure
ctest --test-dir build -R '^RAII\.' --output-on-failure
ctest --test-dir build -R '^UniquePtr\.' --output-on-failure
```

Or run the GoogleTest executable directly:

```sh
./build/tests/unit_tests --gtest_list_tests
./build/tests/unit_tests --gtest_filter='RAII.*'
./build/tests/unit_tests --gtest_filter='UniquePtr.CustomDeleterIsCalled'
```

Unit tests are linked with AddressSanitizer on AppleClang, Clang, and GCC. For
additional leak detection:

```sh
ASAN_OPTIONS=detect_leaks=1 ctest --test-dir build --output-on-failure
```

## Compile Shaders

GLSL sources live in `assets/shaders/`. Compile them to local SPIR-V outputs
with `glslc`:

```sh
glslc assets/shaders/triangle.vert -o assets/shaders/triangle.vert.spv
glslc assets/shaders/triangle.frag -o assets/shaders/triangle.frag.spv
```

Compiled `*.spv` files are ignored by Git. The current executable does not yet
load these shaders.

## CLion

Open the repository as a CMake project and let CLion finish the initial CMake
reload so that `FetchContent` populates GLFW and GLM.

Build and run the `vulkan_practice` CMake target. Do not use the temporary
`C/C++ File.main.cpp` configuration: compiling `main.cpp` in isolation does
not receive CMake's fetched GLFW include directory, and CLion will report:

```text
GLFW/glfw3.h file not found
```

If that warning remains after selecting the CMake target, use
**Tools > CMake > Reload CMake Project**.

## Troubleshooting

### `GLFW/glfw3.h` cannot be found

GLFW is fetched during CMake configuration. Confirm that configuration has run
and that the generated compile command contains GLFW's include directory:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
grep -n 'glfw-src/include' build/compile_commands.json
```

Then build the CMake target, rather than compiling `src/main.cpp` directly:

```sh
cmake --build build --target vulkan_practice
```

### AddressSanitizer runtime cannot be loaded on macOS

After an Xcode or Command Line Tools update, an older test executable may refer
to an AddressSanitizer runtime from the previous AppleClang version. Reconfigure
and rebuild the test executable:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target unit_tests
ctest --test-dir build --output-on-failure
```

### Static-analysis output is noisy

`cppcheck` may also analyze fetched dependency code such as GLM and
GoogleTest. To focus on building and testing the project:

```sh
cmake -S . -B build-fast -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_CLANG_TIDY=OFF -DENABLE_CPPCHECK=OFF
cmake --build build-fast
```
