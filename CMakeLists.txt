cmake_minimum_required(VERSION 3.16)

project(dd
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)"
        FORCE
    )
endif()

file(GLOB_RECURSE dd_SOURCES
    CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(dd ${dd_SOURCES})

# FOR TESTS:
# add_executable(test_object_store
#     tests/test_object_store.cpp
#     src/object.cpp
#     src/hash.cpp
# )

# ------------------------------------------------------------
# Include directories
# ------------------------------------------------------------
target_include_directories(dd
    PRIVATE
        ${PROJECT_SOURCE_DIR}/inc
)

# FOR TESTS:
# target_include_directories(test_object_store
#     PRIVATE
#         ${PROJECT_SOURCE_DIR}/inc
# )

# Mark sha1 as a SYSTEM (external) header-only library
target_include_directories(dd
    SYSTEM PRIVATE
        ${PROJECT_SOURCE_DIR}/third_party/sha1
)

# FOR TESTS:
# target_include_directories(test_object_store
#     SYSTEM PRIVATE
#         ${PROJECT_SOURCE_DIR}/third_party/sha1
# )

# ------------------------------------------------------------
# Compiler warnings and debug flags
# ------------------------------------------------------------
target_compile_options(dd
    PRIVATE
        -ggdb
        -g3
        -O0

        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Werror
)

# ------------------------------------------------------------
# Platform-specific sanity checks
# ------------------------------------------------------------
if(NOT UNIX OR APPLE)
    message(FATAL_ERROR "This project targets Linux/UNIX systems only.")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ standard: C++${CMAKE_CXX_STANDARD}")
