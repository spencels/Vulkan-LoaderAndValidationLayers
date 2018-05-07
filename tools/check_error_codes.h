#ifndef TOOLS_CHECK_ERROR_CODES_H_
#define TOOLS_CHECK_ERROR_CODES_H_

#include <unordered_map>
#include <vector>

#include "clang_headers.h"


class ValidationDatabaseEntry {
 public:
  std::string name;
  bool tested;
  std::string errorString;

  ValidationDatabaseEntry(const std::string& name_, bool tested_, const std::string& errorString_)
      : name(name_), tested(tested_), errorString(errorString_) {}
};


typedef std::unordered_map<std::string, ValidationDatabaseEntry> ErrorCodeMap;


class ErrorMessage {
public:
  uint32_t lineNumber;
  std::string message;

  ErrorMessage(uint32_t l, std::string m) : lineNumber(l), message(std::move(m)) {}
};


class ToolResults {
public:
  std::vector<ErrorMessage> errorMessages;
  clang::tooling::AtomicChanges changes;
};



// Loads relevant information from the validation database.
ErrorCodeMap LoadValidationDatabase(const char* path);


// Creates clang action factory for action that replaces validation layer error strings with error IDs/enums.
std::unique_ptr<clang::tooling::FrontendActionFactory>
NewReplaceErrorStringsActionFactory(ErrorCodeMap *map, ToolResults *results);


#endif // TOOLS_CHECK_ERROR_CODES_H_
