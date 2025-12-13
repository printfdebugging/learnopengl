cmake_minimum_required(VERSION 4.0)
include("${CMAKE_SOURCE_DIR}/cmake/common.cmake")

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
    ${COMPILER_FLAGS}
)


target_compile_definitions(loader
    PRIVATE   LOADER_EXPORT
    INTERFACE LOADER_IMPORT
)
