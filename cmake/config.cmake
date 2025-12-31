cmake_minimum_required(VERSION 4.0)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

if (EMSCRIPTEN)
    set(BUILD_SHARED_LIBS OFF)
    set(CGLM_SHARED OFF)
    set(GLFW_LIBRARY_TYPE STATIC)
else()
    set(BUILD_SHARED_LIBS ON)
    set(CGLM_SHARED OFF) # for SIMD optimizations
    set(GLFW_LIBRARY_TYPE SHARED)
endif()

if (WIN32)
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_GENERATOR "NSIS")
endif()

set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# common target
add_library(common INTERFACE)

target_compile_definitions(common INTERFACE
    $<IF:$<CONFIG:Debug>,-DDEBUG_BUILD,-DRELEASE_BUILD>
)

target_compile_options(common INTERFACE
    -fPIC
    -Wpedantic
    -Werror
    -Wall
    -Wextra
)
