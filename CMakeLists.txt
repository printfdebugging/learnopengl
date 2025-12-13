cmake_minimum_required(VERSION 4.0)
include("${CMAKE_SOURCE_DIR}/cmake/common.cmake")

project(LearnOpenGL
    VERSION         "0.0.1.1"
    DESCRIPTION     "Cross Platform LearnOpenGL Implementaion"
    HOMEPAGE_URL    "https://codeberg.org/printfdebugging/learnopengl"
    LANGUAGES       "C"
)

# external libraries
add_subdirectory(libs/glad)
target_compile_options(glad PRIVATE -fPIC)
target_compile_definitions(glad PRIVATE GLAD_API_CALL_EXPORT)
target_compile_definitions(glad INTERFACE GLAD_API_CALL_EXPORT GLAD_API_CALL_EXPORT_BUILD)

add_subdirectory(libs/cglm)
add_subdirectory(cmake/stb)

# TODO: use generator expression here
if (EMSCRIPTEN)
    add_subdirectory(libs/emscripten-glfw)
    add_library(glfw ALIAS glfw3)
else()
    add_subdirectory(libs/glfw)
    # no add library here as libs/glfw exports glfw
endif()

# NOTE: this is specially helpful when building with
#       emscripten as then we get proper completion
#       and error messages for emscripten bits.
# GH:   https:// github.com/clangd/clangd/issues/1621

if (EMSCRIPTEN)
    execute_process(
        COMMAND ${CMAKE_C_COMPILER} --cflags
        OUTPUT_VARIABLE CLANGD_FLAGS_TO_ADD
    )
else()
    # just add an empty string, compile_commands.json is sufficient
    set(CLANGD_FLAGS_TO_ADD "")
endif()

separate_arguments(CLANGD_FLAGS_TO_ADD UNIX_COMMAND "${CLANGD_FLAGS_TO_ADD}")
list(JOIN CLANGD_FLAGS_TO_ADD ", " CLANGD_FLAGS_TO_ADD)
set(CLANGD_TEMPLATE ${CMAKE_SOURCE_DIR}/scripts/clangd.in)
configure_file(${CLANGD_TEMPLATE} "${CMAKE_SOURCE_DIR}/.clangd")

# internal libs/modules/units
add_subdirectory(source/core)
add_subdirectory(source/loader)

# applications
add_subdirectory(source/tutorials/hello-triangle)
