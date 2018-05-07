if (NOT Clang_FOUND)
    message(FATAL_ERROR "Clang libtooling is required to build tools.")
endif ()


# Separate code into a lib so it's testable.
add_library(check_error_codes_lib STATIC check_error_codes.cpp)
target_link_libraries(check_error_codes_lib clang)
# Disable warning 4996, which pops up when including Clang headers regardless of whether "#pragma warn(push, 0)" is used.
target_compile_options(check_error_codes_lib PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:/wd4996>")


add_executable(check_error_codes check_error_codes_main.cpp)
target_link_libraries(check_error_codes check_error_codes_lib)
