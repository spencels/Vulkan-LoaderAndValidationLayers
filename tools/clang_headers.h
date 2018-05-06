#ifndef TOOLS_CLANG_HEADERS_H_
#define TOOLS_CLANG_HEADERS_H_

#ifdef _MSC_VER
  // Clang headers do not compile without errors in MSVC, so disable them.
  #pragma warning(push, 0)  // Disable warnings
#endif  // _MSC_VER  

#include <clang/Tooling/Refactoring/RefactoringAction.h>
#include <clang/Tooling/Refactoring/RefactoringActionRules.h>
#include <clang/Tooling/Tooling.h>

#ifdef _MSC_VER
  #pragma warning(pop)
#endif  // _MSC_VER

#endif  // TOOLS_CLANG_HEADERS_H_