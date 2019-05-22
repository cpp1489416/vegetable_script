#ifndef VEGETABLE_SCRIPT_CORE_PARSING_LL1_EXPRESSION_ANALYZER_H_
#define VEGETABLE_SCRIPT_CORE_PARSING_LL1_EXPRESSION_ANALYZER_H_

#include <initializer_list>
#include <memory>
#include "./lexer.h"
#include "../ast/core_asts.h"

namespace vegetable_script {

namespace detail {
struct LL1ExpressionAnalyzerResultError {
  using Ptr = std::shared_ptr<LL1ExpressionAnalyzerResultError>;
  using List = std::vector<Ptr>;

  LL1ExpressionAnalyzerResultError(std::string status, int row, int column):
  status(status), row(row), column(column) {
  }

  std::string status;
  int row;
  int column;
};

struct LL1ExpressionAnalyzerResult {
  using Ptr = std::shared_ptr<LL1ExpressionAnalyzerResult>;
  using Error = LL1ExpressionAnalyzerResultError;

  Expression::Ptr expression;
  Error::Ptr error;
};
}  // namespace detail

class LL1ExpressionAnalyzer {
 public:
  using Self = LL1ExpressionAnalyzer;
  using Result = detail::LL1ExpressionAnalyzerResult;
  using Error = Result::Error;

  explicit LL1ExpressionAnalyzer(Lexer::Ptr lexer);

  Result::Ptr Parse();

 private:
  using ParsingFunction =
      Expression::Ptr(Self::*)(Error::List* errors);

  Expression::Ptr ParseExpression(Error::List* errors);

  Expression::Ptr ParsePlusExpression(Error::List* errors);

  Expression::Ptr ParsePlusExpression(Error::List* errors);

  Expression::Ptr ParseMultiplyExpression(Error::List* errors);

  Expression::Ptr ParseNegativeExpression(Error::List* errors);

  Expression::Ptr ParseBinaryExpression(
      ParsingFunction child_function,
      std::initializer_list<Token::Type> token_types,
      std::initializer_list<BinaryExpression::Operator> operator_types,
      Error::List* errors);

  Expression::Ptr ParseUnaryExpression(Error::List* errors);

  Lexer::Ptr lexer_;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSING_LL1_EXPRESSION_ANALYZER_H_
