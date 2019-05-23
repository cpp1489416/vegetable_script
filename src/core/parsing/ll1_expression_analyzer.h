#ifndef VEGETABLE_SCRIPT_CORE_PARSING_LL1_EXPRESSION_ANALYZER_H_
#define VEGETABLE_SCRIPT_CORE_PARSING_LL1_EXPRESSION_ANALYZER_H_

#include <initializer_list>
#include <memory>
#include <deque>
#include "./lexer.h"
#include "../ast/core_asts.h"

namespace vegetable_script {

namespace details {
struct LL1ExpressionAnalyzerError {
  using Ptr = std::shared_ptr<LL1ExpressionAnalyzerError>;
  using List = std::deque<LL1ExpressionAnalyzerError>;

  LL1ExpressionAnalyzerError(std::string status, int row, int column):
  status(status), row(row), column(column) {
  }

  std::string status;
  int row;
  int column;
};

}  // namespace details

class LL1ExpressionAnalyzer {
 public:
  using Self = LL1ExpressionAnalyzer;
  using Error = details::LL1ExpressionAnalyzerError;

  explicit LL1ExpressionAnalyzer(bool on_error_skip = true);

  Expression::Ptr Parse(Lexer* lexer, Error::List* errors);

  bool on_error_skip() const { return on_error_skip_; }

  void set_on_error_skip(bool on_error_skip) { on_error_skip_ = on_error_skip; }

 private:
  using ParsingFunction =
      Expression::Ptr(Self::*)(Lexer* lexer, Error::List* errors);

  Expression::Ptr ParseExpression(Lexer* lexer, Error::List* errors);

  Expression::Ptr ParsePlusExpression(Lexer* lexer, Error::List* errors);

  Expression::Ptr ParseMultiplyExpression(Lexer* lexer, Error::List* errors);

  Expression::Ptr ParseBinaryExpression(
      ParsingFunction child_function,
      std::initializer_list<Token::Type> token_types,
      std::initializer_list<BinaryExpression::Operator> operator_types,
      Lexer* lexer,
      Error::List* errors);

  Expression::Ptr ParseUnaryExpression(Lexer* lexer, Error::List* errors);

  Expression::Ptr ParseSingleExpression(Lexer* lexer, Error::List* errors);

  bool on_error_skip_;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSING_LL1_EXPRESSION_ANALYZER_H_
