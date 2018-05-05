# Find LLVM libraries.
# Exports:
# LLVM_INCLUDE_DIR
# LLVM_LIB_DIR
# LLVM_LIBRARIES
# LLVM_CONFIG_PROGRAM

#find_package(ZLIB REQUIRED)
#if (NOT ZLIB_FOUND)
#    set(LLVM_FOUND OFF)
#    return()
#endif()

#find_package(Curses REQUIRED)
#if (NOT CURSES_FOUND)
#    set(LLVM_FOUND OFF)
#    return()
#endif()

if (EXISTS ${CMAKE_SOURCE_DIR}/external/llvm)
  message(STATUS "Using Clang build in ${CMAKE_SOURCE_DIR}/external/build-llvm.")
  find_program(LLVM_CONFIG_PROGRAM
      llvm-config
      HINTS ${CMAKE_SOURCE_DIR}/external/llvm/build/install
      NO_DEFAULT_PATH
      PATH_SUFFIXES bin
      ) 
else()
  find_program(LLVM_CONFIG_PROGRAM
      llvm-config
      PATHS /usr/local/opt  # Mac OS Homebrew install path
      HINTS ${LLVM_CONFIG_PROGRAM_PATH}
      PATH_SUFFIXES llvm/bin
      ) 
endif()
if (NOT LLVM_CONFIG_PROGRAM)
    message(STATUS "llvm-config program NOT found")
    return()
endif()

# Use llvm-config to find lib and include paths.
execute_process(
    COMMAND ${LLVM_CONFIG_PROGRAM} --includedir
    OUTPUT_VARIABLE LLVM_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
execute_process(
    COMMAND ${LLVM_CONFIG_PROGRAM} --libdir
    OUTPUT_VARIABLE LLVM_LIB_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
set(ClangTooling_INCLUDE_DIRS ${LLVM_INCLUDE_DIR})

execute_process(
    COMMAND ${LLVM_CONFIG_PROGRAM} --libs
    OUTPUT_VARIABLE LLVM_LIB_NAMES
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

if (WIN32)
    set(LLVM_LIB_PATHS ${LLVM_LIB_NAMES})
else()
    string(REGEX REPLACE
        "-l(LLVM[a-zA-Z0-9]+)"
        "${LLVM_LIB_DIR}\\\\${CMAKE_STATIC_LIBRARY_PREFIX}\\1${CMAKE_STATIC_LIBRARY_SUFFIX}"
        LLVM_LIB_PATHS ${LLVM_LIB_NAMES})
endif()
string(REPLACE " " ";" LLVM_LIB_PATHS ${LLVM_LIB_PATHS})
foreach (LIB ${LLVM_LIB_PATHS})
    list(APPEND LLVM_LIBRARIES "${LIB}")
endforeach()

find_package(PackageHandleStandardArgs REQUIRED)
find_package_handle_standard_args(LLVM
    DEFAULT_MSG
    LLVM_INCLUDE_DIR
    LLVM_LIB_DIR
    LLVM_LIBRARIES
    LLVM_CONFIG_PROGRAM
    )

if (NOT LLVM_FOUND)
    return()
endif()

add_library(llvm INTERFACE)
target_include_directories(llvm INTERFACE ${LLVM_INCLUDE_DIRS})
target_link_libraries(llvm INTERFACE ${LLVM_LIBRARIES} ${ZLIB_LIBRARIES} ${CURSES_LIBRARIES})

