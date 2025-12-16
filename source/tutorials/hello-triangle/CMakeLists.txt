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

set(PROJECT_ASSETS_DIR     "${PROJECT_NAME}/assets/")
set(APPLICATION_ASSETS_DIR "${PROJECT_NAME}/${APPLICATION}/assets/")

target_compile_definitions(${APPLICATION} PRIVATE
    GLFW_INCLUDE_NONE
    $<IF:$<PLATFORM_ID:Emscripten>,ASSETS_DIR="${APPLICATION_ASSETS_DIR}",ASSETS_DIR="$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/assets/>$<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_DATAROOTDIR}/${APPLICATION_ASSETS_DIR}>">
    $<IF:$<PLATFORM_ID:Emscripten>,COMMON_ASSETS_DIR="${PROJECT_ASSETS_DIR}",COMMON_ASSETS_DIR="$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/assets/>$<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_DATAROOTDIR}/${PROJECT_ASSETS_DIR}>">
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
        --embed-file=${CMAKE_CURRENT_LIST_DIR}/assets@${APPLICATION_ASSETS_DIR}
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
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/${APPLICATION}"
    )
endif()

add_custom_target(run
    COMMAND ${APPLICATION}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${APPLICATION}
    COMMENT "Running ${APPLICATION} from root directory"
)
