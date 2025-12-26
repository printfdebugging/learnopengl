cmake_minimum_required(VERSION 4.0)
set(MODULE loader)

add_library(${MODULE}
    source/shader.c
    source/texture.c
    source/mesh.c
)

target_include_directories(${MODULE}
    PUBLIC include
)

target_link_libraries(${MODULE}
    PUBLIC core
    PUBLIC stb
)

target_compile_options(${MODULE} PUBLIC
    ${COMPILER_FLAGS}
)

if (EMSCRIPTEN)
    target_compile_definitions(${MODULE}
        PRIVATE LOADER_STATIC
    )
else()
    target_compile_definitions(${MODULE}
        PRIVATE   LOADER_EXPORT
        INTERFACE LOADER_IMPORT
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
