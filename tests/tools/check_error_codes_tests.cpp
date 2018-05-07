#include <string>
#include <gtest/gtest.h>

#include "check_error_codes.h"

namespace tooling = clang::tooling;


class ReplaceErrorStringsTest : public testing::Test {
protected:
  ErrorCodeMap map;
  ToolResults results;
  tooling::AtomicChanges expectedChanges;

  bool Process(const char *code) {
    std::string fullCode = R"(
      #include <cstdint>
      class ErrorMonitor {
       public:
        void SetDesiredFailureMessage(uint32_t flags, const char *const myString) {}
      };
      ErrorMonitor *m_errorMonitor;
      void ContainerFunction() {
    )";
    fullCode += code;
    fullCode += "}";

    auto factory = NewReplaceErrorStringsActionFactory(&map, &results);
    return tooling::runToolOnCode(factory->create(), fullCode.c_str());
  }

  void AddDatabaseEntry(const char *name, bool tested, const char *message) {
    map.emplace(std::make_pair(name, ValidationDatabaseEntry { name, tested, message }));
  }

};


TEST_F(ReplaceErrorStringsTest, ErrorMessageMatchesDatabase) {
  AddDatabaseEntry("ERROR_CODE", false, "Error message.");
  char *code = R"(
    m_errorMonitor->SetDesiredFailureMessage(1, "Error message.");
  )";
  ASSERT_TRUE(Process(code));

  ASSERT_EQ(results.changes.size(), 1);
  ASSERT_EQ(results.changes[0].getReplacements().size(), 1);
  auto replacement = *results.changes[0].getReplacements().begin();
  ASSERT_EQ(replacement.toString(), "");
}


TEST_F(ReplaceErrorStringsTest, ConcatenatedStrings) {
  AddDatabaseEntry("ERROR_CODE", false, "Error message.");
  char *code = R"(
    m_errorMonitor->SetDesiredFailureMessage(1, "Error " "message.");
  )";
  EXPECT_TRUE(Process(code));
}


TEST_F(ReplaceErrorStringsTest, UnknownErrorMessage) {
  AddDatabaseEntry("ERROR_CODE", false, "Error message.");
  char *code = R"(
    m_errorMonitor->SetDesiredFailureMessage(1, "Unknown.");
  )";
  EXPECT_TRUE(Process(code));
}
