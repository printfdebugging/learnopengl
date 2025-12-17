add_library(stb_image stb_image.c)
target_include_directories(stb_image PUBLIC ../../libs/stb)

add_library(stb_include stb_include.c)
target_include_directories(stb_include PUBLIC ../../libs/stb)
target_compile_options(stb_include PRIVATE -Wno-unused-result)

add_library(stb INTERFACE)
target_link_libraries(stb INTERFACE stb_image stb_include)

install(
    DIRECTORY ../../libs/stb
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    TARGETS stb_image stb_include
    EXPORT stbTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
