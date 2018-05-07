#include "check_error_codes.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "clang_headers.h"

namespace tooling = clang::tooling;
namespace ast_matchers = clang::ast_matchers;


namespace {

// Clang statement matcher, which matches on statements like:
//   m_errorMonitor->SetDesiredFailureMsg(<any>, "Hard-coded string");
//
// and binds the AST node of the hard-coded string to "errorMessage".
const auto ERROR_MESSAGE_MATCHER =
    ast_matchers::cxxMemberCallExpr(  // Match member call (x->y() or x.y())
      // Where call is on a class of type "ErrorMonitor *"...
      ast_matchers::on(
        ast_matchers::hasType(
          ast_matchers::pointsTo(
            ast_matchers::cxxRecordDecl(
              ast_matchers::hasName("ErrorMonitor"))))),
      
      // ...method name is "SetDesiredFailureMessage"...
      ast_matchers::callee(
        ast_matchers::cxxMethodDecl(
          ast_matchers::hasName("SetDesiredFailureMessage"))),

      // ...and second argument is a string literal.
      ast_matchers::hasArgument(1, ast_matchers::stringLiteral().bind("errorMessage")));


// Handles AST matches.
class ReplaceErrorStringCallback : public ast_matchers::MatchFinder::MatchCallback {
public:
  ReplaceErrorStringCallback(ErrorCodeMap *map, ToolResults *results)
    : map_(map), results_(results) {}

  virtual void run(const ast_matchers::MatchFinder::MatchResult &result) {
    const auto sourceManager = result.SourceManager;

    // Check for match
    const auto stringLiteral = result.Nodes.getNodeAs<clang::StringLiteral>("errorMessage");
    if (!stringLiteral) return;

    // Check error message string against map of known error messages.
    const auto mapIterator = map_->find(stringLiteral->getString().str());
    if (mapIterator == map_->end()) {
      // Hard-coded error string does not match one in the database.

      // An "expansion" line number refers to the line number source code
      // appears on, before the preprocessor expands macros.
      results_->errorMessages.push_back({
        sourceManager->getExpansionLineNumber(stringLiteral->getLocStart()),
        "Hard-coded error string does not correspond to one in the database."
        });
      return;
    }

    // Create AtomicChange.
    auto dbEntry = mapIterator->second;
    tooling::AtomicChange change{ sourceManager->getFilename(stringLiteral->getLocStart()), dbEntry.name };
    clang::CharSourceRange range;
    range.setBegin(stringLiteral->getLocStart());
    range.setEnd(stringLiteral->getLocEnd());
    change.replace(*sourceManager, range, dbEntry.name);

    results_->changes.push_back(std::move(change));
  }

private:
  ErrorCodeMap *map_;
  ToolResults *results_;
};


// TODO: write me
class ReplaceErrorStringsFactory : public tooling::FrontendActionFactory {
public:
  ReplaceErrorStringsFactory(ErrorCodeMap *map, ToolResults *results)
      : callback_(map, results) {
    finder_.addMatcher(ERROR_MESSAGE_MATCHER, &callback_);
  }

  virtual clang::FrontendAction *create() override {
    class Action : public clang::ASTFrontendAction {
     public:
       Action(ast_matchers::MatchFinder *finder) : finder_(finder) {}

      virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &ci, llvm::StringRef inFile) override {
        return finder_->newASTConsumer();
      }

    private:
      ast_matchers::MatchFinder *finder_;
    };

    return new Action(&finder_);
  }

private:
  ReplaceErrorStringCallback callback_;
  ast_matchers::MatchFinder finder_;
};

}  // namespace


// Loads relevant information from the validation database.
std::unordered_map<std::string, ValidationDatabaseEntry> LoadValidationDatabase(const char* path) {
  std::unordered_map<std::string, ValidationDatabaseEntry> database;

  std::ifstream file(path);

  std::string line;
  const std::string delimiter("~^~");
  while (std::getline(file, line)) {
    if (line[0] == '#') continue;

    llvm::SmallVector<llvm::StringRef, 12> pieces;
    llvm::StringRef(line).split(pieces, delimiter);
    ValidationDatabaseEntry entry(pieces[0], pieces[1] == "Y", pieces[6]);
    database.insert({pieces[6], entry});
  }

  return database;
}


std::unique_ptr<tooling::FrontendActionFactory>
NewReplaceErrorStringsActionFactory(ErrorCodeMap *map, ToolResults *results) {
  return llvm::make_unique<ReplaceErrorStringsFactory>(map, results);
}
