# TODO: this is just me trying things out,
# this isn't the best solution of-course.

cmake_minimum_required(VERSION 4.0)
project(executable)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    ${PROJECT_NAME}
    source/main.c
    source/logger.c
    source/window.c
    source/shader.c
    source/stb.c
)

include("${CMAKE_SOURCE_DIR}/cmake/config.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/scripts.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/externals.cmake")

include("${CMAKE_SOURCE_DIR}/cmake/custom.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/assets.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/install.cmake")
