cmake_minimum_required(VERSION 4.0)
include("${CMAKE_SOURCE_DIR}/cmake/common.cmake")

add_library(loader SHARED
    source/shader.c
)

target_include_directories(loader
    PUBLIC include
)

target_link_libraries(loader
    PUBLIC core
    PUBLIC stb
)

target_compile_options(loader PUBLIC
    ${COMPILER_FLAGS}
)


target_compile_definitions(loader
    PRIVATE   LOADER_EXPORT
    INTERFACE LOADER_IMPORT
)

install(
    DIRECTORY include/loader
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    TARGETS loader
    EXPORT  loader
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
