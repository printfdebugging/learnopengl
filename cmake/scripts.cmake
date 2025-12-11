cmake_minimum_required(VERSION 4.0)

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
