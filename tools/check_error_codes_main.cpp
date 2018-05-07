#include <iostream>

#include "check_error_codes.h"
#include "clang_headers.h"

namespace tooling = clang::tooling;

const char* kValidationDatabasePath = "/Users/spencels/src/vulkan-layers/layers/vk_validation_error_database.txt";
const char* kLayersValidationTest = "/Users/spencels/src/vulkan-layers/tests/layer_validation_tests.cpp";

static llvm::cl::OptionCategory CheckErrorCodesOptions("check_error_codes options");


// Entry point
int main(int argc, const char** argv) {
  tooling::CommonOptionsParser optionsParser(argc, argv, CheckErrorCodesOptions);
  tooling::ClangTool tool(optionsParser.getCompilations(), optionsParser.getSourcePathList());

  // Load validation database.
  auto database = LoadValidationDatabase(kValidationDatabasePath);

  ToolResults results;
  auto returnCode = tool.run(NewReplaceErrorStringsActionFactory(&database, &results).get());
  if (returnCode) {
    std::cout << "Shit failed with code " << returnCode << "\n";
    return returnCode;
  }

  std::cout << "ERRORS:\n\n";
  for (auto &message : results.errorMessages) {
    std::cout << message.lineNumber << ": " << message.message << "\n";
  }
  std::cout << "\n";

  std::cout << "REPLACEMENTS:\n\n";
  for (auto &change : results.changes) {
    std::cout << change.getReplacements().begin()->toString() << "\n";
  }

  return 0;
}