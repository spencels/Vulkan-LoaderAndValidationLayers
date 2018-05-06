#include "check_error_codes.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "clang_headers.h"

//namespace tooling = clang::tooling;


// Split std::string into chunks by delimiter.
std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter) {
  std::vector<std::string> parts;

  auto input = str.c_str();
  size_t length = str.size();
  while (true) {
    auto delimiter_start = strstr(input, delimiter.c_str());
    if (!delimiter_start) break;

    parts.push_back(std::string(input, delimiter_start - input));
    const auto difference = delimiter_start - input + delimiter.size();
    length -= difference;
    input += difference;
  }

  parts.push_back(std::string(input, length));

  return parts;
}


// Loads relevant information from the validation database.
std::unordered_map<std::string, ValidationDatabaseEntry> LoadValidationDatabase(const char* path) {
  std::unordered_map<std::string, ValidationDatabaseEntry> database;

  std::ifstream file(path);

  std::string line;
  const std::string delimiter("~^~");
  while (std::getline(file, line)) {
    if (line[0] == '#') continue;

    auto pieces = SplitString(line, delimiter);
    ValidationDatabaseEntry entry(pieces[0], pieces[1] == "Y", pieces[6]);
    database.insert({pieces[6], entry});
  }

  return database;
}
