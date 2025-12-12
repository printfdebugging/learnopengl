cmake_minimum_required(VERSION 4.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_Executable(hello-triangle
    source/main.c
)

target_include_directories(hello-triangle
    PUBLIC ../../../include
)

target_link_libraries(hello-triangle
    core
    loader
)

target_compile_options(hello-triangle PUBLIC
    "${PROJECT_CMAKE_C_FLAGS}"
)

# TODO: use generator expression instead of conditional
if (EMSCRIPTEN)
    target_compile_definitions(hello-triangle PRIVATE
        GLFW_INCLUDE_NONE
        ASSET_DIR="assets/"
    )
else()
    target_compile_definitions(hello-triangle PRIVATE
        GLFW_INCLUDE_NONE
        ASSET_DIR="$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/assets/>$<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_DATAROOTDIR}/${PROJECT_NAME}/assets/>"
    )
endif()
