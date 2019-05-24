#include <iostream>
#include "../core/visitor/ast_stringer.h"
#include "../core/parsing/all.h"
#include "../core/parsing/ll1_expression_analyzer.h"

void EpochStatementFile(const std::string& path) {
  using namespace vegetable_script;  // NOLINT
  const char* source_code = R"ABC(
    1 * 2)ABC";

  auto source_provider = std::make_shared<SourceProvider>(
      SourceProvider::FromFile(path));
  Lexer lexer_print = Lexer(source_provider);
  for (int i = 0; lexer_print.HasNext(); ++i, lexer_print.MoveNext()) {
    ParsingException exception;
    Token token;
    if (!lexer_print.LookCurrentWithoutComments(&token, &exception)) {
      std::cout << exception.ToString("errors when lexical analysing")
          << std::endl;
      break;
    }
    std::cout << i << "th token: " << token.ToEscapedString() << std::endl;
  }

  std::cout << std::endl;

  source_provider = std::make_shared<SourceProvider>(
      SourceProvider::FromFile(path));
  Lexer lexer = Lexer(source_provider);
  LL1ExpressionAnalyzer expression_analyzer;
  for (int i = 0; true; ++i) {
    Expression::Ptr expression;
    ParsingException exception;
    if (!expression_analyzer.Parse(&lexer, &expression, &exception)) {
      std::cout << exception.ToString("errors when parsing") << std::endl;
      break;
    }
    AstStringer ast_stringer;
    expression->Accept(&ast_stringer);
    std::cout << i << "th expression: " << ast_stringer.Result() << std::endl;
  }

}


XC_TEST_CASE(statemenet_analyzer, true) {
  using namespace vegetable_script;  // NOLINT
  std::string file_directory = xc::FileUtil::GetDirectory(__FILE__) + "sample/";
  std::cout << file_directory << std::endl;
  std::string path = file_directory + "expression.inc";
  EpochStatementFile(path);
  /*
  if (result->error != nullptr) {
    std::cout << "some errors occured\n";
  } else {
    auto expression = result->expression;
    AstStringer ast_stringer;
    expression->Accept(&ast_stringer);
    std::cout << ast_stringer.Result() << std::endl;
  }
  std::cout << "eat" << std::endl;
  */
}
