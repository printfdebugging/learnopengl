CMAKE_MINIMUM_REQUIRED(VERSION 4.0)
PROJECT(executable)

INCLUDE(FetchContent)

set(C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)
# add_subdirectory(libs/stb)
add_subdirectory(libs/cglm)



add_executable(
    ${PROJECT_NAME}
    source/main.c
    source/logger.c
    source/window.c
    source/shader.c
    source/stb.c
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -std=c11
    -g
    -Wall
    -Wextra
    -static
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    glfw
    cglm
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    GLFW_INCLUDE_NONE
    ASSET_DIR="$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/assets/>$<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_DATAROOTDIR}/${PROJECT_NAME}/assets/>"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/stb
    ${CMAKE_SOURCE_DIR}/include
)

add_custom_target(run
    COMMAND ${PROJECT_NAME} 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${PROJECT_NAME} 
    COMMENT "Running ${PROJECT_NAME} from root directory"
)

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
