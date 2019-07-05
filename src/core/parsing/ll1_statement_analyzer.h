#ifndef VEGETABLE_SCRIPT_CORE_PARSING_LL1_STATEMENT_ANALYZER_H_
#define VEGETABLE_SCRIPT_CORE_PARSING_LL1_STATEMENT_ANALYZER_H_

#include <initializer_list>
#include <memory>
#include <deque>
#include "./lexer.h"
#include "../ast/core_asts.h"
#include "./parsing_exception.h"
#include "./ll1_expression_analyzer.h"

namespace vegetable_script {

class Ll1StatementAnalyzer{
 public:
  using Self = Ll1StatementAnalyzer;
  using Exception = ParsingException;

  bool Parse(Lexer* lexer, Statement::Ptr* statement, Exception* exception);

  bool ParseStatement(Lexer* lexer, Statement::Ptr* statement, Exception* exception);

  bool ParseExpressionStatement(Lexer* lexer, ExpressionStatement::Ptr* statement, Exception* exception);

  bool ParseDefinitionStatement(Lexer* lexer, Definition::Ptr* definition, Exception* expression);

  bool ParseBlockStatement(Lexer* lexer, BlockStatement::Ptr* statement, Exception* exception);

  bool ParseIfStatement(Lexer* lexer, IfStatement::Ptr* statement, Exception* exception);

  bool ParseWhileStatement(Lexer* lexer, WhileStatement::Ptr* statement, Exception* exception);

  bool ParseForStatement(Lexer* lexer, ForStatement::Ptr* statement, Exception* exception);

  Ll1ExpressionAnalyzer expression_analyzer_;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSING_LL1_STATEMENT_ANALYZER_H_

