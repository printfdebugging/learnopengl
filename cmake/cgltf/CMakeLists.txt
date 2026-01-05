add_library(cgltf
    cgltf.c
    cgltf_write.c
)

target_include_directories(cgltf PUBLIC ../../libs/cgltf)

install(
    DIRECTORY ../../libs/cgltf
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    TARGETS cgltf
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
