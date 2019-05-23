
#include <iostream>
#include <vegetable_script/mul.h>
#include <common/all.h>
#include <parsing/all.h>
#include <parsing/ll1_expression_analyzer.h>
#include <visitor/ast_stringer.h>


XC_TEST_CASE(statemenet_analyzer, false) {
  using namespace vegetable_script;  // NOLINT
  const char* source_code =
#include "./expression.inc"
;  // NOLINT
  std::cout << "moving\n";
  auto source_provider = std::make_shared<SourceProvider>(source_code);
  Lexer lexer_print = Lexer(source_provider);
  Lexer lexer = Lexer(std::make_shared<SourceProvider>(*source_provider));
  std::cout << "moving";
  while (lexer_print.HasNext()) {
    auto result = lexer_print.LookCurrent(errors);

    std::cout << result->token->ToString() << std::endl;
    lexer_print.MoveNext();
  }
  auto result = LL1ExpressionAnalyzer().Parse(&lexer);
  if (result->error != nullptr) {
    std::cout << "some errors occured\n";
  } else {
    auto expression = result->expression;
    AstStringer ast_stringer;
    expression->Accept(&ast_stringer);
    std::cout << ast_stringer.Result() << std::endl;
  }
  std::cout << "eat" << std::endl;
}
