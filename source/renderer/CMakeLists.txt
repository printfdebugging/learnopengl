cmake_minimum_required(VERSION 4.0)
set(MODULE renderer)

add_library(${MODULE}
    source/mesh.c
    source/text.c
)

target_include_directories(${MODULE}
    PUBLIC include
)

target_link_libraries(${MODULE}
    PUBLIC loader
    PUBLIC freetype
    PRIVATE common
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

