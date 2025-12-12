cmake_minimum_required(VERSION 4.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

project(
    executable
    VERSION         "0.0.1.1"
    DESCRIPTION     "Cross Platform LearnOpengl Implementaion"
    HOMEPAGE_URL    "https://codeberg.org/printfdebugging/learnopengl"
    LANGUAGES       "C"
)


# TODO: add WIN32/MACOSX flags here so that the stdout
# dialogs don't show up when the application runs
add_executable(
    ${PROJECT_NAME}
    source/main.c
    source/logger.c
    source/window.c
    source/shader.c
    source/stb.c
)

# TODO: is this the best solution? probably not. include the original
# cmake files here, separate them into nice sections, separated by 
#comments

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# TODO: separate this -g flag for debug and release builds
target_compile_options(${PROJECT_NAME} PRIVATE
    -std=c11
    -g
    -Wall
    -Wextra
    -static
)


# NOTE: this is specially helpful when building with
#       emscripten as then we get proper completion
#       and error messages for emscripten bits.
# GH:   https:// github.com/clangd/clangd/issues/1621

if (EMSCRIPTEN)
    execute_process(COMMAND ${CMAKE_C_COMPILER} --cflags OUTPUT_VARIABLE CLANGD_FLAGS_TO_ADD)
    separate_arguments(CLANGD_FLAGS_TO_ADD UNIX_COMMAND "${CLANGD_FLAGS_TO_ADD}")
    list(JOIN CLANGD_FLAGS_TO_ADD ", " CLANGD_FLAGS_TO_ADD)
    set(CLANGD_TEMPLATE ${CMAKE_SOURCE_DIR}/scripts/clangd.in)
    configure_file(${CLANGD_TEMPLATE} "${CMAKE_SOURCE_DIR}/.clangd")
else()
    # TODO: delete the .clangd file or else it will have the
    # build flags from the last run. just replace the placeholder with
    # -g or some cflags for warnings and errors.
endif()


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
add_subdirectory("${CMAKE_SOURCE_DIR}/libs/glad")
add_subdirectory("${CMAKE_SOURCE_DIR}/libs/cglm")

if (EMSCRIPTEN)
    # use emscripten-glfw instead of glfw
    add_subdirectory("${CMAKE_SOURCE_DIR}/libs/emscripten-glfw")
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw3)

    # generate html
    # TODO part of this should go to config.cmake part should go to assets.cmake
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
        -sMAX_WEBGL_VERSION=2 \
        -sNO_DISABLE_EXCEPTION_CATCHING \
        -s ASSERTIONS=1 \
        -s WASM=1 \
        -s SAFE_HEAP=1 \
        --preload-file ${CMAKE_SOURCE_DIR}/assets/shaders/shader.vert \
        --preload-file ${CMAKE_SOURCE_DIR}/assets/shaders/shader.frag \
    ")
else()
    add_subdirectory("${CMAKE_SOURCE_DIR}/libs/glfw")
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


if (EMSCRIPTEN)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        GLFW_INCLUDE_NONE
        ASSET_DIR="assets/"
    )
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        GLFW_INCLUDE_NONE
        ASSET_DIR="$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/assets/>$<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_DATAROOTDIR}/${PROJECT_NAME}/assets/>"
    )
endif()


# TODO: look into if these are valid for the emscripten build
# or are these just for the regular build
install(
    TARGETS ${PROJECT_NAME}
    EXPORT  ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY "${CMAKE_SOURCE_DIR}/assets"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}"
)

add_custom_target(run
    COMMAND ${PROJECT_NAME} 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${PROJECT_NAME} 
    COMMENT "Running ${PROJECT_NAME} from root directory"
)
