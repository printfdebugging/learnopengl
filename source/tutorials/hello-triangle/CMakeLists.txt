cmake_minimum_required(VERSION 4.0)
include("${CMAKE_SOURCE_DIR}/cmake/common.cmake")
set(APPLICATION "hello-triangle")

add_executable(${APPLICATION}
    source/main.c
)

target_include_directories(${APPLICATION}
    PUBLIC include
)

target_link_libraries(${APPLICATION}
    core
    loader
)

target_compile_options(${APPLICATION} PUBLIC
    ${COMPILER_FLAGS}
)

# TODO: use generator expression instead of conditional
if (EMSCRIPTEN)
    target_compile_definitions(${APPLICATION} PRIVATE
        GLFW_INCLUDE_NONE
        ASSET_DIR="assets/"
    )
else()
    target_compile_definitions(${APPLICATION} PRIVATE
        GLFW_INCLUDE_NONE
        ASSET_DIR="$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/assets/>$<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_DATAROOTDIR}/${PROJECT_NAME}/${APPLICATION}/assets/>"
    )
endif()

install(
    TARGETS ${APPLICATION}
    EXPORT  ${APPLICATION}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/assets"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/${APPLICATION}"
)
