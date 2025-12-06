cmake_minimum_required(VERSION 4.0)

# TODO: load all the assets in just one preload call.
# TODO: move this to assets
set(AssetFiles
    assets/shaders/shader.vert
    assets/shaders/shader.frag
    assets/textures/awesomeface.png
    assets/textures/container.jpg
    assets/textures/wall.jpg
    assets/logo.png
)


# all this should be in externals.cmake
add_subdirectory(libs/glad)
add_subdirectory(libs/cglm)

if (EMSCRIPTEN)
    # use emscripten-glfw instead of glfw
    add_subdirectory(libs/emscripten-glfw)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw3)

    set(linkerFlags "-sMAX_WEBGL_VERSION=2")
    set(linkerFlags "${linkerFlags} -sNO_DISABLE_EXCEPTION_CATCHING")
    set(linkerFlags "${linkerFlags} -s ASSERTIONS=1")
    set(linkerFlags "${linkerFlags} -s WASM=1")
    set(linkerFlags "${linkerFlags} -s SAFE_HEAP=1")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${linkerFlags}")


    # generate html
    # TODO part of this should go to config.cmake part should go to assets.cmake
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${CMAKE_BINARY_DIR}/assets/shaders/shader.vert --preload-file ${CMAKE_BINARY_DIR}/assets/shaders/shader.frag")
else()
    add_subdirectory(libs/glfw)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
endif()


target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    cglm
)


target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/stb
    ${CMAKE_SOURCE_DIR}/include
)
