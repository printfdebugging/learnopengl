cmake_minimum_required(VERSION 4.0)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(loader SHARED
    source/shader.c
)

target_include_directories(loader
    PUBLIC include
)

target_link_libraries(loader
    glad
    glfw
    core
    stb
)

target_compile_options(loader PUBLIC
    ${PROJECT_CMAKE_C_FLAGS}
)

