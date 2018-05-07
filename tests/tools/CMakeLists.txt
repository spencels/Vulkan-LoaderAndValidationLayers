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
    clang
    )
# Disable warning 4996, which pops up when including Clang headers regardless of whether "#pragma warn(push, 0)" is used.
target_compile_options(tools_tests PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:/wd4996>")

if (WIN32)
    # For Windows, copy necessary gtest DLLs to the right spot for the vk_layer_tests...
    FILE(TO_NATIVE_PATH ${CMAKE_BINARY_DIR}/submodules/googletest/googletest/$<CONFIGURATION>/*.dll SRC_PATH)
    FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION> DST_GTEST_DLLS)
    add_custom_command(TARGET tools_tests POST_BUILD
        COMMAND xcopy /Y /I ${SRC_PATH} ${DST_GTEST_DLLS})
endif()