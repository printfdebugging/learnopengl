cmake_minimum_required(VERSION 4.0)
project(shooter)

set(C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

ADD_SUBDIRECTORY(libs/glad)
ADD_SUBDIRECTORY(libs/glfw)
ADD_SUBDIRECTORY(libs/cglm)

ADD_EXECUTABLE(
    shooter
    source/main.c
    source/window.c
    source/shader.c
    source/stb.c
)

TARGET_COMPILE_OPTIONS(shooter PRIVATE
    -std=c11
    -g
    -Wall
    -Wextra
    -static
)

TARGET_LINK_LIBRARIES(shooter PRIVATE
    glad
    glfw
    cglm
)

TARGET_COMPILE_DEFINITIONS(shooter PRIVATE
    GLFW_INCLUDE_NONE
)

TARGET_INCLUDE_DIRECTORIES(shooter PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/stb
    ${CMAKE_SOURCE_DIR}/include
)

add_custom_target(run
    COMMAND shooter
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS shooter
    COMMENT "Running shooter from root directory"
)
