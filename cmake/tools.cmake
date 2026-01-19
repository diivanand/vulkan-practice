# cmake/tools.cmake
function(_find_tool out_var tool_name)
  find_program(${out_var} NAMES ${tool_name})
endfunction()

if (ENABLE_CLANG_TIDY)
  _find_tool(CLANG_TIDY_EXE clang-tidy)
  if (CLANG_TIDY_EXE)
    # DO NOT write to cache / do not FORCE
    set(CMAKE_CXX_CLANG_TIDY
            "${CLANG_TIDY_EXE};--format-style=file"
    )
  else()
    message(STATUS "clang-tidy not found; disabling clang-tidy.")
    set(CMAKE_CXX_CLANG_TIDY "")
  endif()
endif()

if (ENABLE_CPPCHECK)
  _find_tool(CPPCHECK_EXE cppcheck)
  if (CPPCHECK_EXE)
    set(CMAKE_CXX_CPPCHECK
            "${CPPCHECK_EXE};--enable=warning,style,performance,portability;--inline-suppr;--suppress=missingIncludeSystem"
    )
  else()
    message(STATUS "cppcheck not found; disabling cppcheck.")
    set(CMAKE_CXX_CPPCHECK "")
  endif()
endif()
