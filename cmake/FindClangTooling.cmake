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
        message(FATAL_ERROR "Could not find ${_libname}")
    endif ()
    set(ClangTooling_LIBRARIES
        ${ClangTooling_LIBRARIES}
        ${ClangTooling_${_libname}_LIB}
        )
endmacro()

find_program(LLVM_CONFIG
    llvm-config
    PATHS /usr/local/opt  # Mac OS Homebrew install path
    PATH_SUFFIXES llvm/bin
    ) 
if (NOT LLVM_CONFIG)
    message(FATAL_ERROR "llvm-config program not found")
endif()

# Use llvm-config to find libs.
execute_process(
  COMMAND ${LLVM_CONFIG} --includedir
  OUTPUT_VARIABLE LLVM_INCLUDE_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
execute_process(
  COMMAND ${LLVM_CONFIG} --libdir
  OUTPUT_VARIABLE LLVM_LIB_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

find_path(ClangTooling_INCLUDE_DIRS
    NAMES clang/Tooling/Tooling.h
    HINTS ${LLVM_INCLUDE_DIR}
    )

find_and_add_clang_library(LLVM)
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