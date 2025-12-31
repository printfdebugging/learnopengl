cmake_minimum_required(VERSION 4.0)

project(LearnOpenGL
    VERSION         "0.0.1.1"
    DESCRIPTION     "Cross Platform LearnOpenGL Implementaion"
    HOMEPAGE_URL    "https://codeberg.org/printfdebugging/learnopengl"
    LANGUAGES       "C"
)

# NOTE: include config after project command as that is when the
#       compiler specific symbols are guaranteed to exist.
include("${CMAKE_SOURCE_DIR}/cmake/config.cmake")

# external libraries
add_subdirectory(libs/glad)
add_subdirectory(libs/cglm)
add_subdirectory(libs/freetype2)
add_subdirectory(cmake/stb)

if (EMSCRIPTEN)
    add_subdirectory(libs/emscripten-glfw)
    add_library(glfw ALIAS glfw3)
else()
    add_subdirectory(libs/glfw)
endif()

# internal libs/modules/units
add_subdirectory(source/core)
add_subdirectory(source/loader)
add_subdirectory(source/renderer)

# applications
add_subdirectory(source/game)

# misc
add_subdirectory(cmake/graphviz)        # dependency graphs
add_subdirectory(cmake/clangd)          # .clangd
add_subdirectory(cmake/assets)          # assets
