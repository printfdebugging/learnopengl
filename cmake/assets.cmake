cmake_minimum_required(VERSION 4.0)

add_custom_target(CopyAssets
	COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${CMAKE_SOURCE_DIR}/assets" 
        "${CMAKE_BINARY_DIR}/assets"
    COMMENT "Copying Assets"
)

add_dependencies(${PROJECT_NAME} CopyAssets)


# TODO move this to assets
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


