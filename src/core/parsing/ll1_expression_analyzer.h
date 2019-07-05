#ifndef VEGETABLE_SCRIPT_CORE_PARSING_Ll1_EXPRESSION_ANALYZER_H_
#define VEGETABLE_SCRIPT_CORE_PARSING_Ll1_EXPRESSION_ANALYZER_H_

#include <initializer_list>
#include <memory>
#include <deque>
#include "./lexer.h"
#include "../ast/core_asts.h"
#include "./parsing_exception.h"

namespace vegetable_script {

class Ll1ExpressionAnalyzer {
 public:
  using Self = Ll1ExpressionAnalyzer;
  using Exception = ParsingException;

  bool Parse(Lexer* lexer, Expression::Ptr* expression, Exception* exception);

 private:
  using ParsingFunction = bool (Self::*)(Lexer* lexer, Expression::Ptr* expression, Exception* exception);

  bool ParseExpression(Lexer* lexer, Expression::Ptr* expression, Exception* exception);
  bool ParseCommaExpression(Lexer* lexer, Expression::Ptr* expression, Exception* exception);
  bool ParseAssignExpression(Lexer* lexer, Expression::Ptr* expression, Exception* exception);
  bool ParseOrExpression(Lexer* lexer, Expression::Ptr* expression, Exception* exception);
  bool ParseAndExpression(Lexer* lexer, Expression::Ptr* expression, Exception* exception);
  bool ParseEqualExpression(Lexer* lexer, Expression::Ptr* expression, Exception* exception);
  bool ParsePlusExpression(Lexer* lexer, Expression::Ptr* expression, Exception* exception);
  bool ParseMultiplyExpression(Lexer* lexer, Expression::Ptr* expression, Exception* exception);

  bool ParseBinaryExpressionFromLeftToRight(
      ParsingFunction child_function,
      std::initializer_list<Token::Type> token_types,
      std::initializer_list<BinaryExpression::Operator> operator_types,
      Lexer* lexer,
      Expression::Ptr* expression,
      Exception* exception);

  bool ParseBinaryExpressionFromRightToLeft(
      ParsingFunction child_function,
      std::initializer_list<Token::Type> token_types,
      std::initializer_list<BinaryExpression::Operator> operator_types,
      Lexer* lexer,
      Expression::Ptr* expression,
      Exception* exception);

  bool ParseUnaryExpression(Lexer* lexer, Expression::Ptr* expression, Exception* exception);
  bool ParseSingleExpression(Lexer* lexer, Expression::Ptr* expression, Exception* exception);
  bool ParseFunctionInvokeExpression(Lexer* lexer, Expression::Ptr* expression, Exception* excetion);
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSING_Ll1_EXPRESSION_ANALYZER_H_
