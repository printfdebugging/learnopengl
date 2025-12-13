cmake_minimum_required(VERSION 4.0)
include("${CMAKE_SOURCE_DIR}/cmake/common.cmake")

add_library(core SHARED
    source/logger.c
    source/window.c
)

target_include_directories(core PUBLIC include)

target_link_libraries(core
    glad
    glfw
    cglm
)

target_compile_options(core PUBLIC
    ${COMPILER_FLAGS}
)

target_compile_definitions(core
    PRIVATE   CORE_EXPORT
    INTERFACE CORE_IMPORT
    INTERFACE GLAD_API_CALL_EXPORT
)

install(
    DIRECTORY include/core
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    TARGETS core
    EXPORT  core
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
