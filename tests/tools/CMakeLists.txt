file(GLOB CPP_FILES *.cpp)

include_directories("${PROJECT_SOURCE_DIR}/tools")

add_executable(tools_tests ${CPP_FILES})
target_link_libraries(tools_tests
    gtest
    gtest_main
    check_error_codes_lib
    )