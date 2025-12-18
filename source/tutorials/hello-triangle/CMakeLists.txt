cmake_minimum_required(VERSION 4.0)
include(GNUInstallDirs)
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

    if (USE_GENERATED_INDEX_HTML AND GENERATED_GRAPHICS_DIR_PATH)
        set(GENERATED_INDEX_HTML_FILE "${GENERATED_GRAPHICS_DIR_PATH}/${APPLICATION}/index.html")
    endif()
    set(TEMPLATE_INDEX_HTML_FILE "${PROJECT_SOURCE_DIR}/scripts/index.html")

    target_link_options(${APPLICATION} PRIVATE
        -sMAX_WEBGL_VERSION=2
        -sNO_DISABLE_EXCEPTION_CATCHING
        -sASSERTIONS=1
        -sWASM=1
        -sSAFE_HEAP=1
        --embed-file=${PROJECT_SOURCE_DIR}/assets@${PROJECT_ASSETS_DIR}
        --embed-file=${CMAKE_CURRENT_LIST_DIR}/assets@${APPLICATION_ASSETS_DIR}
        $<IF:$<BOOL:${USE_GENERATED_INDEX_HTML}>,--shell-file=${GENERATED_INDEX_HTML_FILE},--shell-file=${TEMPLATE_INDEX_HTML_FILE}>
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

if (WIN32)
    # copy the DLLs to the build directory POST_BUILD
    add_custom_command(
        TARGET ${APPLICATION} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${APPLICATION}> ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Copy DLLs to the ${APPLICATION} directory"
        COMMAND_EXPAND_LISTS
    )

    # copy the DLLs to the binary installation directory
    # and also to the LIB directory for completion
    install(FILES $<TARGET_RUNTIME_DLLS:${APPLICATION}> TYPE BIN)
    install(FILES $<TARGET_RUNTIME_DLLS:${APPLICATION}> TYPE LIB)
endif()
