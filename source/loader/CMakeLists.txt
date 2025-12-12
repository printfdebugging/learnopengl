cmake_minimum_required(VERSION 4.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(loader STATIC #SHARED
    shader.c
    stb.c
)

target_include_directories(loader
    PUBLIC ../../libs/stb
    PUBLIC ../../include/loader
)

target_link_libraries(loader
    glad
    glfw
    core
)

target_compile_options(loader PUBLIC
    "${PROJECT_CMAKE_C_FLAGS}"
)

