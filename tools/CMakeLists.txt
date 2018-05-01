if (NOT ClangTooling_FOUND)
    message(FATAL_ERROR "Clang libtooling is required to build tools.")
endif ()

include_directories(${ClangTooling_INCLUDE_DIRS} SYSTEM)

# Separate code into a lib so it's testable.
add_library(check_error_codes_lib STATIC check_error_codes.cpp)
target_link_libraries(check_error_codes_lib PRIVATE ${ClangTooling_LIBRARIES})

add_executable(check_error_codes check_error_codes_main.cpp)
target_link_libraries(check_error_codes check_error_codes_lib)
