cmake_minimum_required(VERSION 4.0)
project(glad)

add_library(glad
    src/glad.c
)

target_include_directories(glad
    PUBLIC include
)

install(
    DIRECTORY 
        include/glad 
        include/KHR
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    TARGETS glad
    EXPORT  glad
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
