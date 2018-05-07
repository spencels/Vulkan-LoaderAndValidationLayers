#ifndef TOOLS_CLANG_HEADERS_H_
#define TOOLS_CLANG_HEADERS_H_

// Clang headers do not compile without errors in MSVC, so disable them.
#ifdef _MSC_VER
  #pragma warning(push, 0)  // Disable warnings
#endif  // _MSC_VER  

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/Refactoring/AtomicChange.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/ADT/StringRef.h>

#ifdef _MSC_VER
  #pragma warning(pop)
#endif  // _MSC_VER

#endif  // TOOLS_CLANG_HEADERS_H_