#ifndef VEGETABLE_SCRIPT_CORE_PARSING_LL1_DEFINITION_ANALYZER_H_
#define VEGETABLE_SCRIPT_CORE_PARSING_LL1_DEFINITION_ANALYZER_H_

#include <initializer_list>
#include <memory>
#include <deque>
#include "./lexer.h"
#include "../ast/core_asts.h"
#include "./parsing_exception.h"
#include "./ll1_expression_analyzer.h"

namespace vegetable_script {

class Ll1DefinitionAnalyzer{
 public:
  using Self = Ll1DefinitionAnalyzer;
  using Exception = ParsingException;

  bool Parse(Lexer* lexer, Definition::Ptr* definition, Exception* exception);

 private:
  bool ParseDefinition(Lexer* lexer, Definition::Ptr* statement, Exception* exception);

  bool ParseFunctionDefinition(Lexer* lexer, ExpressionStatement::Ptr* statement, Exception* exception);

  bool ParseVariableDefinition(Lexer* lexer, BlockStatement::Ptr* statement, Exception* exception);

  Ll1ExpressionAnalyzer expression_analyzer_;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSING_LL1_DEFINITION_ANALYZER_H_
