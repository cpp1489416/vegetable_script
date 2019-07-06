#include <iostream>
#include <typeinfo>
#include <string>
#include "../core/visitor/ast_stringer.h"
#include "../core/parsing/all.h"
#include "../core/parsing/ll1_expression_analyzer.h"
#include "../core/parsing/ll1_statement_analyzer.h"
#include "../core/running/all.h"

namespace {
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

  BlockStatement::Ptr block_statement = std::make_shared<BlockStatement>();
  source_provider = std::make_shared<SourceProvider>(
      SourceProvider::FromFile(path));
  Lexer lexer = Lexer(source_provider);
  Ll1StatementAnalyzer statement_analyzer;
  for (int i = 0; true; ++i) {
    Statement::Ptr statement;
    ParsingException exception;
    if (!statement_analyzer.Parse(&lexer, &statement, &exception)) {
      std::cout << exception.ToString("errors when parsing") << std::endl;
      break;
    }
    block_statement->statements.push_back(statement);

    AstStringer ast_stringer;
    statement->Accept(&ast_stringer);
    std::cout << i << "th expression: " << ast_stringer.Result() << typeid(*statement.get()).name() << std::endl;
  }

  std::cout << "start to invoke" << std::endl;
  ScopeStack scope_stack;
  NativeFunctionSymbol::FillScopeStack(&scope_stack);
  CustomFunctionSymbol::FillScopeStack(block_statement.get(), &scope_stack);
  DoubleStatementInvoker double_statement_invoker(&scope_stack);
  block_statement->Accept(&double_statement_invoker);
  if (!double_statement_invoker.success()) {
    std::cout << double_statement_invoker.exception().status << std::endl;
  }
}
}  // namespace


XC_TEST_CASE(double_invoker, true) {
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
