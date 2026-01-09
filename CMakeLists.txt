cmake_minimum_required(VERSION 4.0)

project(LearnOpenGL
    VERSION         "0.0.1.1"
    DESCRIPTION     "Cross Platform LearnOpenGL Implementaion"
    HOMEPAGE_URL    "https://codeberg.org/printfdebugging/learnopengl"
    LANGUAGES       "C"
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(BUILD_SHARED_LIBS OFF)
set(CGLM_SHARED OFF)
set(GLFW_LIBRARY_TYPE STATIC)

if (WIN32)
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_GENERATOR "NSIS")
endif()

set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# external libraries
add_subdirectory(libs/glad)
add_subdirectory(libs/cglm)
add_subdirectory(libs/freetype2)
add_subdirectory(cmake/stb)
add_subdirectory(cmake/cgltf)

if (EMSCRIPTEN)
    add_subdirectory(libs/emscripten-glfw)
    add_library(glfw ALIAS glfw3)
else()
    add_subdirectory(libs/glfw)
endif()

# misc
add_subdirectory(cmake/graphviz)        # dependency graphs
add_subdirectory(cmake/clangd)          # .clangd
add_subdirectory(cmake/assets)          # assets

# globals
set(APPLICATION "executable")
set(TEMPLATE_INDEX_HTML_FILE "${PROJECT_SOURCE_DIR}/scripts/index.html")
set(PROJECT_ASSETS_DIR "${PROJECT_NAME}/assets/")

# source
add_executable(${APPLICATION}
    source/logger.c
    source/main.c
    source/mesh.c
    source/renderer.c
    source/shader.c
    source/texture.c
    source/window.c
)

target_compile_definitions(${APPLICATION} PRIVATE
    GLFW_INCLUDE_NONE
    $<IF:$<CONFIG:Debug>,-DDEBUG_BUILD,-DRELEASE_BUILD>
    $<IF:$<PLATFORM_ID:Emscripten>,ASSETS_DIR="${PROJECT_ASSETS_DIR}",ASSETS_DIR="$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/assets/>$<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_DATAROOTDIR}/${PROJECT_ASSETS_DIR}>">
)

target_compile_options(${APPLICATION} PRIVATE
    -fPIC
    -Wpedantic
    -Werror
    -Wall
    -Wextra
)

target_include_directories(${APPLICATION}
    PUBLIC include
)

target_link_libraries(${APPLICATION} PUBLIC
    glad
    glfw
    cgltf
    cglm
    stb
    freetype
    m
)

if (EMSCRIPTEN)
    set_target_properties(${APPLICATION} PROPERTIES OUTPUT_NAME "index")
    set_target_properties(${APPLICATION} PROPERTIES SUFFIX ".html")

    target_link_options(${APPLICATION} PRIVATE
        -sMAX_WEBGL_VERSION=2
        -sNO_DISABLE_EXCEPTION_CATCHING
        -sASSERTIONS=1
        -sWASM=1
        -sSAFE_HEAP=1
        --embed-file=${PROJECT_SOURCE_DIR}/assets@${PROJECT_ASSETS_DIR}
        --shell-file=${TEMPLATE_INDEX_HTML_FILE}
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/index.html
            ${CMAKE_CURRENT_BINARY_DIR}/index.js
            ${CMAKE_CURRENT_BINARY_DIR}/index.wasm
        DESTINATION ${CMAKE_INSTALL_BINDIR}/${APPLICATION}
    )
else()
    install(
        TARGETS ${APPLICATION}
        EXPORT  ${APPLICATION}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    install(
        DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/assets"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}"
    )
endif()
