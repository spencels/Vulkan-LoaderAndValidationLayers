# Find Clang libtooling development headers and library.
# Exports: 
# Clang_INCLUDE_DIRS
# Clang_LIB_DIRS
# Clang_LIBRARIES

macro (find_and_add_clang_library _libname)
    find_library(Clang_${_libname}_LIB
        NAMES ${_libname}
        HINTS ${LLVM_LIB_DIR}
        )
    if (Clang_${_libname}_LIB)
        message(STATUS "Found ${_libname}")
    else()
        message(STATUS "Could not find ${_libname}")
        set(Clang_FOUND OFF)
        return()
    endif ()
    list(APPEND Clang_LIBRARIES
        ${Clang_${_libname}_LIB}
        )
endmacro()

find_package(LLVM REQUIRED)
set(Clang_INCLUDE_DIRS ${LLVM_INCLUDE_DIR})
set(Clang_LIB_DIRS ${LLVM_LIB_DIR})

# Version 5+ required.
execute_process(
  COMMAND ${LLVM_CONFIG_PROGRAM} --version
  OUTPUT_VARIABLE LLVM_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

if (LLVM_VERSION VERSION_LESS "5")
  set(Clang_FOUND OFF)
  message(WARNING "LLVM version ${LLVM_VERSION} < 5.0")
  return()
endif()

# Add clang libraries.
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
find_package_handle_standard_args(Clang
    DEFAULT_MSG Clang_INCLUDE_DIRS Clang_LIB_DIRS Clang_LIBRARIES)

if (NOT Clang_FOUND)
    return()
endif()

add_library(clang INTERFACE IMPORTED)
set_property(TARGET clang PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Clang_INCLUDE_DIRS})
target_link_libraries(clang INTERFACE llvm ${Clang_LIBRARIES})