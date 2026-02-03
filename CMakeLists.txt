cmake_minimum_required(VERSION 3.16)

# ------------------------------------------------------------
# Project definition
# ------------------------------------------------------------
project(dd
    VERSION 0.1.0
    LANGUAGES CXX
)

# ------------------------------------------------------------
# Global C++ settings
# ------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json for clangd / tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to Debug build if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)"
        FORCE
    )
endif()

# ------------------------------------------------------------
# Source files
# ------------------------------------------------------------
file(GLOB_RECURSE dd_SOURCES
    CONFIGURE_DEPENDS
    src/*.cpp
)

# ------------------------------------------------------------
# Executable target
# ------------------------------------------------------------
add_executable(dd ${dd_SOURCES})

# ------------------------------------------------------------
# Include directories
# ------------------------------------------------------------
target_include_directories(dd
    PRIVATE
        ${PROJECT_SOURCE_DIR}/inc
)

# ------------------------------------------------------------
# Compiler warnings and debug flags (your Makefile flags)
# ------------------------------------------------------------
target_compile_options(dd
    PRIVATE
        # Debugging
        -ggdb
        -g3
        -O0

        # Warnings
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion

        # Treat warnings as errors
        -Werror
)

# ------------------------------------------------------------
# Platform-specific sanity checks (Linux-only project)
# ------------------------------------------------------------
if(NOT UNIX OR APPLE)
    message(FATAL_ERROR "This project targets Linux/UNIX systems only.")
endif()


# ------------------------------------------------------------
# Final output info
# ------------------------------------------------------------
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ standard: C++${CMAKE_CXX_STANDARD}")
