cmake_minimum_required(VERSION 4.0)
project(executable)
include(FetchContent)

set(C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CPACK_GENERATOR "TGZ")

FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git    
    GIT_TAG origin/master
)

FetchContent_Declare(cglm
    GIT_REPOSITORY https://github.com/recp/cglm.git
    GIT_TAG origin/master
)

FetchContent_Declare(glad
    GIT_REPOSITORY https://github.com/nitrix/glad.git
    GIT_TAG origin/master
)

FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG origin/master
)

FetchContent_MakeAvailable(
    glfw
    cglm
    glad
    stb
)

ADD_EXECUTABLE(
    executable
    source/main.c
    source/logger.c
    source/window.c
    source/shader.c
    source/stb.c
)

TARGET_COMPILE_OPTIONS(executable PRIVATE
    -std=c11
    -g
    -Wall
    -Wextra
    -static
)

TARGET_LINK_LIBRARIES(executable PRIVATE
    glad
    glfw
    cglm
)

TARGET_COMPILE_DEFINITIONS(executable PRIVATE
    GLFW_INCLUDE_NONE
)

TARGET_INCLUDE_DIRECTORIES(executable PRIVATE
    ${FETCHCONTENT_BASE_DIR}/stb-src
    ${CMAKE_SOURCE_DIR}/include
)

ADD_CUSTOM_TARGET(run
    COMMAND executable
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS executable
    COMMENT "Running executable from root directory"
)
