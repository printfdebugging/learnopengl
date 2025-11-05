cmake_minimum_required(VERSION 4.0)
project(learnopengl)

set(C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

ADD_SUBDIRECTORY(libs/glad)
ADD_SUBDIRECTORY(libs/glfw)

ADD_EXECUTABLE(
    learnopengl
    src/main.c
    src/stb.c
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
)

TARGET_COMPILE_DEFINITIONS(learnopengl PRIVATE
    GLFW_INCLUDE_NONE
)

TARGET_INCLUDE_DIRECTORIES(learnopengl PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/stb
)

add_custom_target(run
    COMMAND learnopengl
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS learnopengl
    COMMENT "Running learnopengl from root directory"
)
