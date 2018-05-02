# Find LLVM libraries.
# Exports:
# LLVM_INCLUDE_DIR
# LLVM_LIB_DIR
# LLVM_LIBRARIES
# LLVM_CONFIG_PROGRAM

find_package(zlib REQUIRED)
if (NOT ZLIB_FOUND)
    set(LLVM_FOUND OFF)
    return()
endif()
set(LLVM_LIBRARIES ${ZLIB_LIBRARIES})

find_package(curses REQUIRED)
if (NOT CURSES_FOUND)
    set(LLVM_FOUND OFF)
    return()
endif()

list(APPEND LLVM_LIBRARIES ${CURSES_LIBRARIES})

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
    COMMAND ${LLVM_CONFIG} --libdir
    OUTPUT_VARIABLE LLVM_LIB_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
set(ClangTooling_INCLUDE_DIRS ${LLVM_INCLUDE_DIR})

execute_process(
    COMMAND ${LLVM_CONFIG} --libs
    OUTPUT_VARIABLE LLVM_LIB_NAMES
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
string(REGEX REPLACE
    "-l(LLVM[a-zA-Z0-9]+)"
    "${LLVM_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}\\1${CMAKE_STATIC_LIBRARY_SUFFIX}"
    LLVM_LIB_PATHS ${LLVM_LIB_NAMES})
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
