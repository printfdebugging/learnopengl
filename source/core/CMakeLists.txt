cmake_minimum_required(VERSION 4.0)
set(MODULE core)

add_library(${MODULE}
    source/logger.c
    source/window.c
    source/utils/string.c
)

target_include_directories(${MODULE} PUBLIC include)

target_link_libraries(${MODULE}
    PUBLIC glad
    PUBLIC glfw
    PUBLIC cglm
)

target_compile_options(${MODULE} PUBLIC
    ${COMPILER_FLAGS}
)

if(EMSCRIPTEN)
    target_compile_definitions(${MODULE}
        PRIVATE CORE_STATIC
    )
else()
    target_compile_definitions(${MODULE}
        PRIVATE   CORE_EXPORT
        INTERFACE CORE_IMPORT
        PRIVATE   GLAD_GLAPI_EXPORT
        INTERFACE GLAD_GLAPI_EXPORT
    )
    install(
        DIRECTORY include/${MODULE}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        TARGETS ${MODULE}
        EXPORT  ${MODULE}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()
