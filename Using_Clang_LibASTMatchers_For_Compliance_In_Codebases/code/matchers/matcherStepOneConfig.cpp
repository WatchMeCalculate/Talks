#include <iostream>

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

void coutRaiseJson(std::string name)
{
  std::cout << "{ \"type\": \"RaiseCalledOnAlarm\", \"name\": \"" << name << "\" }\n";
}

enum class InstantiationResultType {
  Success, Error
};

using InstantiationResult = std::pair<InstantiationResultType, std::string>;

void coutInstantiatedJson(std::string name, InstantiationResult instantiatedWith)
{
  if (instantiatedWith.first == InstantiationResultType::Error) {
    std::cout
      << "{ \"type\": \"Error\", \"message\": \""
      << instantiatedWith.second
      << "\" }\n";
    return;
  }
  std::cout
     << "{ \"type\": \"AlarmClientInstantiated\", \"name\": \""
     << name
     << "\", \"instantiatedWith\": "
     << instantiatedWith.second
     << " }\n";
}






InstantiationResult readConfigArg(const MatchFinder::MatchResult &Result)
{
  auto *cfgSection = Result.Nodes.getNodeAs<clang::MemberExpr>("cfgSection");
  auto *cfgClass = Result.Nodes.getNodeAs<clang::MemberExpr>("cfgClass");
  auto *strLiteral = Result.Nodes.getNodeAs<clang::StringLiteral>("strLit");
  if (strLiteral)
  {
    return std::make_pair<InstantiationResultType, std::string>(
      InstantiationResultType::Success,
      "{ \"type\": \"StringLiteral\", \"value\": \"" + strLiteral->getString().str() + "\" }");
  }
  if (!cfgSection || !cfgClass) return std::make_pair<InstantiationResultType, std::string>(InstantiationResultType::Error, "Something is wrong");
  std::string fieldName = cfgSection->getMemberDecl()->getNameAsString();
  std::string sectionName = cfgClass->getMemberDecl()->getNameAsString();
  std::string className = cfgClass->getMemberDecl()->getQualifiedNameAsString();
  size_t lastColon = className.find_last_of(":");
  std::string trimmedClassName = className.substr(0, lastColon - 1);
  return std::make_pair<InstantiationResultType, std::string>(
    InstantiationResultType::Success,
    "{ \"type\": \"ConfigValue\", \"classname\": \"" +
    trimmedClassName + "\", \"xmlpath\": \"" +
    sectionName + "." + fieldName + "\" }"
  );
}

DeclarationMatcher LocalAlarmMatcher= varDecl(hasType(asString("Spooky::Factory::AlarmClient")),
        hasAncestor(cxxMethodDecl().bind("method")),
        hasDescendant(memberExpr(hasObjectExpression(hasType(
  cxxRecordDecl(isSameOrDerivedFrom("Spooky::Config::ConfigFile"))
))).bind("cfgClass"))
        ).bind("inst");
class LocalAlarmHandler: public MatchFinder::MatchCallback
{
public:
  virtual void run(const MatchFinder::MatchResult &Result)
  {
    auto *vd = Result.Nodes.getNodeAs<clang::VarDecl>("inst");
    auto *mthd = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method");
    auto alarmInfo = readConfigArg(Result);
    if (!vd || !mthd) return;
    auto varName = vd->getNameAsString();
    auto methodName = mthd->getQualifiedNameAsString();
    coutInstantiatedJson(methodName + "::" + varName, alarmInfo);
  }

};

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

int main(int argc, const char **argv)
{
  auto ExpectedParser =  CommonOptionsParser::create(argc, argv, MyToolCategory);
  if (!ExpectedParser) {
    // Fail gracefully for unsupported options.
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  MatchFinder finder;
  LocalAlarmHandler localAlarmHandler;
  finder.addMatcher(LocalAlarmMatcher, &localAlarmHandler);

  return Tool.run(newFrontendActionFactory(&finder).get());
}
