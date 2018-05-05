file(GLOB CPP_FILES *.cpp)

# LLVM and Clang headers won't build without warnings on Windows.
if (WIN32)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/W3>")
endif()

include_directories("${PROJECT_SOURCE_DIR}/tools")

add_executable(tools_tests ${CPP_FILES})
target_link_libraries(tools_tests
    gtest
    gtest_main
    check_error_codes_lib
    )

if (WIN32)
    # For Windows, copy necessary gtest DLLs to the right spot for the vk_layer_tests...
    FILE(TO_NATIVE_PATH ${PROJECT_BINARY_DIR}/submodules/googletest/googletest/$<CONFIGURATION> SRC_PATH)
    add_custom_command(TARGET tools_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SRC_PATH} $<TARGET_FILE_DIR:tools_tests>)
endif()