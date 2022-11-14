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
project(nagen CXX)

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
find_package(Doxygen)

if(DOXYGEN_FOUND)
    # Doxygen theme
    include(FetchContent)
    FetchContent_Declare(DoxygenAwesome
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
        GIT_TAG 4cd62308d825fe0396d2f66ffbab45d0e247724c # 2.0.3
    )
    FetchContent_MakeAvailable(DoxygenAwesome)
    FetchContent_GetProperties(DoxygenAwesome SOURCE_DIR DoxygenAwesome_SOURCE_DIR)

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_HTML_HEADER ${DoxygenAwesome_SOURCE_DIR}/doxygen-custom/header.html)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${DoxygenAwesome_SOURCE_DIR}/doxygen-awesome.css)
    set(DOXYGEN_HTML_EXTRA_FILES
        ${DoxygenAwesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
        ${DoxygenAwesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js
        ${DoxygenAwesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    )

    doxygen_add_docs(${PROJECT_NAME}_documentation
        include/namegen/namegen.hpp
        LICENSE.md
        README.md
    )
endif()

# -----------------------------------------------------------------------------
# Examples
# -----------------------------------------------------------------------------
if(NAGEN_BUILD_EXAMPLES)
    # Add the example example.
    add_executable(nagen_example example/example.cpp)

    # Inlcude header directories.
    target_include_directories(nagen_example PUBLIC ${PROJECT_SOURCE_DIR}/include)

    # Set compilation flags.
    target_compile_options(nagen_example PUBLIC ${NAGEN_COMPILE_OPTIONS})
endif()
