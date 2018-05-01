# Find Clang libtooling development headers and library.
# TODO: Support options for include and lib dirs.
# TODO: Test on Linux.

macro (find_and_add_clang_library _libname)
    find_library(ClangTooling_${_libname}_LIB
        NAMES ${_libname}
        HINTS ${LLVM_LIB_DIR}
        )
    if (ClangTooling_${_libname}_LIB)
        message(STATUS "Found ${_libname} at ${ClangTooling_${_libname}_LIB}")
    else()
        message(STATUS "Could not find ${_libname}")
        set(ClangTooling_FOUND OFF)
        return()
    endif ()
    set(ClangTooling_LIBRARIES
        ${ClangTooling_LIBRARIES}
        ${ClangTooling_${_libname}_LIB}
        )
endmacro()

if (EXISTS ${CMAKE_SOURCE_DIR}/external/llvm)
  message(STATUS "Using Clang build in ${CMAKE_SOURCE_DIR}/external/build-llvm.")
  find_program(LLVM_CONFIG
      llvm-config
      HINTS ${CMAKE_SOURCE_DIR}/external/llvm/build/install
      NO_DEFAULT_PATH
      PATH_SUFFIXES bin
      ) 
  set(LLVM_INCLUDE_DIR
      ${CMAKE_SOURCE_DIR}/external/llvm/llvm/include
      ${CMAKE_SOURCE_DIR}/external/llvm/build/include
      ${CMAKE_SOURCE_DIR}/external/llvm/llvm/tools/clang/include
      ${CMAKE_SOURCE_DIR}/external/llvm/build/tools/clang/include)
else()
  find_program(LLVM_CONFIG
      llvm-config
      PATHS /usr/local/opt  # Mac OS Homebrew install path
      HINTS ${LLVM_CONFIG_PATH}
      PATH_SUFFIXES llvm/bin
      ) 
  execute_process(
    COMMAND ${LLVM_CONFIG} --includedir
    OUTPUT_VARIABLE LLVM_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()
if (NOT LLVM_CONFIG)
    message(STATUS "llvm-config program NOT found")
    return()
endif()

# Version 5+ required.
execute_process(
  COMMAND ${LLVM_CONFIG} --version
  OUTPUT_VARIABLE LLVM_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

if (LLVM_VERSION VERSION_LESS "5")
  set(ClangTooling_FOUND OFF)
  message(WARNING "LLVM version ${LLVM_VERSION} < 5.0")
  return()
endif()

# Use llvm-config to find libs.
execute_process(
  COMMAND ${LLVM_CONFIG} --libdir
  OUTPUT_VARIABLE LLVM_LIB_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
set(ClangTooling_INCLUDE_DIRS ${LLVM_INCLUDE_DIR})

find_and_add_clang_library(LLVMCore)
find_and_add_clang_library(clangAST)
find_and_add_clang_library(clangAnalysis)
find_and_add_clang_library(clangBasic)
find_and_add_clang_library(clangDriver)
find_and_add_clang_library(clangEdit)
find_and_add_clang_library(clangFrontend)
find_and_add_clang_library(clangLex)
find_and_add_clang_library(clangParse)
find_and_add_clang_library(clangSema)
find_and_add_clang_library(clangSerialization)
find_and_add_clang_library(clangTooling)

find_package(PackageHandleStandardArgs REQUIRED)
find_package_handle_standard_args(ClangTooling
    DEFAULT_MSG ClangTooling_INCLUDE_DIRS ClangTooling_LIBRARIES)
