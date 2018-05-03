#include "check_error_codes.h"

#include <clang/Tooling/Refactoring/RefactoringAction.h>
#include <clang/Tooling/Refactoring/RefactoringActionRules.h>
#include <clang/Tooling/Tooling.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
using namespace clang::tooling;


// Split string into chunks by delimiter.
vector<string> SplitString(const string& str, const string& delimiter) {
  vector<string> parts;

  auto input = str.c_str();
  size_t length = str.size();
  while (true) {
    auto delimiter_start = strstr(input, delimiter.c_str());
    if (!delimiter_start) break;

    parts.push_back(string(input, delimiter_start - input));
    const auto difference = delimiter_start - input + delimiter.size();
    length -= difference;
    input += difference;
  }

  parts.push_back(string(input, length));

  return parts;
}


// Loads relevant information from the validation database.
unordered_map<string, ValidationDatabaseEntry> LoadValidationDatabase(const char* path) {
  unordered_map<string, ValidationDatabaseEntry> database;

  ifstream file(path);

  string line;
  const string delimiter("~^~");
  while (getline(file, line)) {
    if (line[0] == '#') continue;

    auto pieces = SplitString(line, delimiter);
    ValidationDatabaseEntry entry(pieces[0], pieces[1] == "Y", pieces[6]);
    database.insert({pieces[6], entry});
  }

  return database;
}

class ReplaceErrorStringsRule : public SourceChangeRefactoringRule {
};
