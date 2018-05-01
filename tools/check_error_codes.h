#ifndef CHECK_ERROR_CODES_H_
#define CHECK_ERROR_CODES_H_

#include "clang/Tooling/Refactoring/RefactoringAction.h"
#include "clang/Tooling/Refactoring/RefactoringActionRules.h"
#include <clang/Tooling/Tooling.h>
#include <string>
#include <unordered_map>
#include <vector>


// Split string into chunks by delimiter.
std::vector<std::string> SplitString(const std::string_view str, const std::string_view delimiter);


class ValidationDatabaseEntry {
 public:
  std::string name;
  bool tested;
  std::string errorString;

  ValidationDatabaseEntry(const std::string& name_, bool tested_, const std::string& errorString_)
      : name(name_), tested(tested_), errorString(errorString_) {}
};


// Loads relevant information from the validation database.
std::unordered_map<std::string, ValidationDatabaseEntry> LoadValidationDatabase(const char* path);


class ReplaceErrorStringsAction : public clang::tooling::RefactoringAction {
 public:
  llvm::StringRef getCommand() const override {
    return "replace-error-strings";
  }

  llvm::StringRef getDescription() const override {
    return "Replaces hard-coded error strings with the appropriate error code enum.";
  }

};

#endif // CHECK_ERROR_CODES_H_
