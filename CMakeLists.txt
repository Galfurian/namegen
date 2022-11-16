# -----------------------------------------------------------------------------
# @brief  : Root cmake file.
# @author : Enrico Fraccaroli
# @create : 03/01/2022
# @update : 03/01/2022
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Set the minimum CMake version, the project name and default build type.
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1)

# Set the project name.
project(namegen CXX)

# -----------------------------------------------------------------------------
# Add options.
# -----------------------------------------------------------------------------
option(NAGEN_STRICT_WARNINGS "Enable strict compiler warnings" ON)
option(NAGEN_WARNINGS_AS_ERRORS "Treat all warnings as errors" ON)
option(NAGEN_BUILD_EXAMPLES "Build examples" ON)

# -----------------------------------------------------------------------------
# LIBRARY
# -----------------------------------------------------------------------------
# Add the C++ library.
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Inlcude header directories.
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include)

# -----------------------------------------------------------------------------
# Set the compilation flags.
# -----------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable warnings that suggest using MSVC-specific safe functions
    set(NAGEN_COMPILE_OPTIONS "${NAGEN_COMPILE_OPTIONS} -D_CRT_SECURE_NO_WARNINGS")

    if(NAGEN_WARNINGS_AS_ERRORS)
        set(NAGEN_COMPILE_OPTIONS "${NAGEN_COMPILE_OPTIONS} /WX")
    endif(NAGEN_WARNINGS_AS_ERRORS)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(NAGEN_WARNINGS_AS_ERRORS)
        set(NAGEN_COMPILE_OPTIONS "${NAGEN_COMPILE_OPTIONS} -Werror")
    endif(NAGEN_WARNINGS_AS_ERRORS)
endif()

if(NAGEN_STRICT_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # Mark system headers as external for MSVC explicitly
        # https://devblogs.microsoft.com/cppblog/broken-warnings-theory
        set(NAGEN_COMPILE_OPTIONS "${NAGEN_COMPILE_OPTIONS} /experimental:external")
        set(NAGEN_COMPILE_OPTIONS "${NAGEN_COMPILE_OPTIONS} /external:I ${CMAKE_BINARY_DIR}")
        set(NAGEN_COMPILE_OPTIONS "${NAGEN_COMPILE_OPTIONS} /external:anglebrackets")
        set(NAGEN_COMPILE_OPTIONS "${NAGEN_COMPILE_OPTIONS} /external:W0")
        set(NAGEN_COMPILE_OPTIONS "${NAGEN_COMPILE_OPTIONS} /W4")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(NAGEN_COMPILE_OPTIONS "${NAGEN_COMPILE_OPTIONS} -Wall -Wextra -Wconversion -pedantic")
    endif()
endif(NAGEN_STRICT_WARNINGS)

string(REPLACE " " ";" NAGEN_COMPILE_OPTIONS ${NAGEN_COMPILE_OPTIONS})

# -----------------------------------------------------------------------------
# DOCUMENTATION
# -----------------------------------------------------------------------------
set(DOXYGEN_INPUT
    ${PROJECT_SOURCE_DIR}/README.md
    ${PROJECT_SOURCE_DIR}/LICENSE.md
    ${PROJECT_SOURCE_DIR}/include
)
add_subdirectory(docs)

# -----------------------------------------------------------------------------
# Examples
# -----------------------------------------------------------------------------
if(NAGEN_BUILD_EXAMPLES)
    # Add the example example.
    add_executable(namegen_example example/namegen_example.cpp)

    # Inlcude header directories.
    target_include_directories(namegen_example PUBLIC ${PROJECT_SOURCE_DIR}/include)

    # Set compilation flags.
    target_compile_options(namegen_example PUBLIC ${NAGEN_COMPILE_OPTIONS})
endif()
