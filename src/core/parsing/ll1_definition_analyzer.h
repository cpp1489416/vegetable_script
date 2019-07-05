#ifndef VEGETABLE_SCRIPT_CORE_PARSING_LL1_DEFINITION_ANALYZER_H_
#define VEGETABLE_SCRIPT_CORE_PARSING_LL1_DEFINITION_ANALYZER_H_

#include <initializer_list>
#include <memory>
#include <deque>
#include "./lexer.h"
#include "../ast/core_asts.h"
#include "./parsing_exception.h"

namespace vegetable_script {

class Ll1DefinitionAnalyzer{
 public:
  using Self = Ll1DefinitionAnalyzer;
  using Exception = ParsingException;

  bool Parse(Lexer* lexer, Definition::Ptr* definition, Exception* exception);

 private:
  bool ParseDefinition(Lexer* lexer, Definition::Ptr* statement, Exception* exception);

  bool ParseFunctionDefinition(Lexer* lexer, FunctionDefinition::Ptr* statement, Exception* exception);

  bool ParseVariableDefinition(Lexer* lexer, VariableDefinition::Ptr* statement, Exception* exception);
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSING_LL1_DEFINITION_ANALYZER_H_
