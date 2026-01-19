include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
        test_traits_raii.cpp
)

target_link_libraries(unit_tests
  PRIVATE
    GTest::gtest
    GTest::gmock
    GTest::gtest_main
)

target_compile_features(unit_tests PRIVATE cxx_std_20)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
  target_compile_options(unit_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_asan_for_target(unit_tests)

include(GoogleTest)
gtest_discover_tests(unit_tests)