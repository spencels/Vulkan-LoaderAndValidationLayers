#include "check_error_codes.h"

#include "clang/Tooling/Refactoring/RefactoringAction.h"
#include "clang/Tooling/Refactoring/RefactoringActionRules.h"
#include <clang/Tooling/Tooling.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <string_view>
#include <vector>
#include <unordered_map>

using namespace std;
using namespace clang::tooling;


// Split string into chunks by delimiter.
vector<string> SplitString(const string_view str, const string_view delimiter) {
  vector<string> parts;
  auto input = str;
  while (true) {
    auto delimiter_start = input.find_first_of(delimiter);
    if (delimiter_start == string_view::npos) break;

    parts.push_back(string(input.data(), delimiter_start));
    const auto next_start = delimiter_start + delimiter.size();
    input = input.substr(next_start, input.size() - next_start);
  }

  parts.push_back(string(input));

  return parts;
}


// Loads relevant information from the validation database.
unordered_map<string, ValidationDatabaseEntry> LoadValidationDatabase(const char* path) {
  unordered_map<string, ValidationDatabaseEntry> database;

  ifstream file(path);

  string line;
  while (getline(file, line)) {
    if (line[0] == '#') continue;

    auto pieces = SplitString(line, "~^~");
    ValidationDatabaseEntry entry(pieces[0], pieces[1] == "Y", pieces[6]);
    database.insert({pieces[6], entry});
  }

  return database;
}

class ReplaceErrorStringsRule : public SourceChangeRefactoringRule {
};