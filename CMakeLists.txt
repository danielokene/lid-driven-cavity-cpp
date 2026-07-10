cmake_minimum_required(VERSION 3.16)

project(
    LidDrivenCavity
    VERSION 2.0
    DESCRIPTION "2D Lid-Driven Cavity Flow Solver using the Finite Difference Method"
    LANGUAGES CXX
)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler Warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Collect all source files
file(GLOB SRC_FILES
    src/*.cpp
)

# Executable
add_executable(
    LidDrivenCavity
    ${SRC_FILES}
)

# Include directories
target_include_directories(
    LidDrivenCavity
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

# Output executable to project root
set_target_properties(LidDrivenCavity PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)