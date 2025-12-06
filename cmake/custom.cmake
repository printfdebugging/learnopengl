cmake_minimum_required(VERSION 4.0)

add_custom_target(run
    COMMAND ${PROJECT_NAME} 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${PROJECT_NAME} 
    COMMENT "Running ${PROJECT_NAME} from root directory"
)
