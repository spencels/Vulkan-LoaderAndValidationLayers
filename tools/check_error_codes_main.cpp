
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <iostream>
#include <vector>

#include "check_error_codes.h"

using namespace std;
using namespace clang::tooling;

const char* kValidationDatabasePath = "/Users/spencels/src/vulkan-layers/layers/vk_validation_error_database.txt";
const char* kLayersValidationTest = "/Users/spencels/src/vulkan-layers/tests/layer_validation_tests.cpp";

static llvm::cl::OptionCategory CheckErrorCodesOptions("check_error_codes options");


// Entry point
int main(int argc, const char** argv) {
  CommonOptionsParser optionsParser(argc, argv, CheckErrorCodesOptions);

  // RefactoringTool tool(optionsParser.getCompilations(), optionsParser.getSourcePathList());

  // Load validation database.
  auto database = LoadValidationDatabase(kValidationDatabasePath);

  vector<string> sources {kLayersValidationTest};
  return 0;
}