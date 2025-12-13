cmake_minimum_required(VERSION 4.0)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(core SHARED
    source/logger.c
    source/window.c
)

target_include_directories(core PUBLIC include)

target_link_libraries(core
    glad
    glfw
    cglm
)

target_compile_options(core PUBLIC
    ${PROJECT_CMAKE_C_FLAGS}
)
