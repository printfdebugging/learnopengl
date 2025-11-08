cmake_minimum_required(VERSION 4.0)
project(learnopengl)

set(C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

ADD_SUBDIRECTORY(libs/glad)
ADD_SUBDIRECTORY(libs/glfw)
ADD_SUBDIRECTORY(libs/cglm)

ADD_EXECUTABLE(
    learnopengl
    source/main.c
    source/window.c
    source/stb.c
)

TARGET_COMPILE_OPTIONS(learnopengl PRIVATE
    -std=c11
    -g
    -Wall
    -Wextra
    -static
)

TARGET_LINK_LIBRARIES(learnopengl PRIVATE
    glad
    glfw
    cglm
)

TARGET_COMPILE_DEFINITIONS(learnopengl PRIVATE
    GLFW_INCLUDE_NONE
)

TARGET_INCLUDE_DIRECTORIES(learnopengl PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/stb
    ${CMAKE_SOURCE_DIR}/include
)

add_custom_target(run
    COMMAND learnopengl
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS learnopengl
    COMMENT "Running learnopengl from root directory"
)
