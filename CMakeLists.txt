cmake_minimum_required(VERSION 4.0)

project(
    executable
    VERSION         "0.0.1.1"
    DESCRIPTION     "Cross Platform LearnOpengl Implementaion"
    HOMEPAGE_URL    "https://codeberg.org/printfdebugging/learnopengl"
    LANGUAGES       "C"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TODO: add WIN32/MACOSX flags here so that the stdout
# dialogs don't show up when the application runs
add_executable(
    ${PROJECT_NAME}
    source/main.c
    source/logger.c
    source/window.c
    source/shader.c
    source/stb.c
)

# TODO: is this the best solution? probably not. include the original
# cmake files here, separate them into nice sections, separated by 
#comments

include("${CMAKE_SOURCE_DIR}/cmake/config.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/scripts.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/externals.cmake")

include("${CMAKE_SOURCE_DIR}/cmake/custom.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/assets.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/install.cmake")
