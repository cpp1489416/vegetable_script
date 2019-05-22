#include "./ll1_expression_analyzer.h"

namespace vegetable_script {

LL1ExpressionAnalyzer::LL1ExpressionAnalyzer(Lexer::Ptr lexer) :
    lexer_(lexer) {
}

LL1ExpressionAnalyzer::Result::Ptr
LL1ExpressionAnalyzer::Parse() {
  Error::List errors;
  auto expression = ParseExpression(&errors);
  auto result = std::make_shared<Result>();
  result->expression = expression;
  if (!errors.empty()) {
    result->error = errors.front();
  } else {
    result->error = nullptr;
  }
  return result;
}

Expression::Ptr LL1ExpressionAnalyzer::ParseExpression(
    LL1ExpressionAnalyzer::Error::List* errors) {
  return ParsePlusExpression(errors);
}

Expression::Ptr LL1ExpressionAnalyzer::ParsePlusExpression(
    LL1ExpressionAnalyzer::Error::List* errors) {
  auto left = ParseMultiplyExpression(errors);
  auto token = lexer_->LookCurrent()->token;
  if (token->Match({
      Token::Type::kOperatorPlus, Token::Type::kOperatorMinus})) {
    auto node = std::make_shared<CalculateExpression>();
    node->left_expression = left;
    node->operatorr = (
        token->type == Token::Type::kOperatorPlus ?
        CalculateExpression::Operator::kPlus :
        CalculateExpression::Operator::kMinus);
    node->right_expression = ParsePlusExpression(errors);
    return node;
  } else {
    return left;
  }
}

Expression::Ptr LL1ExpressionAnalyzer::ParseMultiplyExpression(
    LL1ExpressionAnalyzer::Error::List* errors) {
  auto left = ParseMultiplyExpression(errors);
  auto token = lexer_->LookCurrent()->token;
  if (token->Match({
      Token::Type::kOperatorMultiply, Token::Type::kOperatorDivide})) {
    auto node = std::make_shared<CalculateExpression>();
    node->left_expression = left;
    node->operatorr = (
        token->type == Token::Type::kOperatorMultiply?
        CalculateExpression::Operator::kMultiply :
        CalculateExpression::Operator::kDivide);
    node->right_expression = ParseMultiplyExpression(errors);
    return node;
  } else {
    return left;
  }
}

Expression::Ptr LL1ExpressionAnalyzer::ParseNegativeExpression(
    LL1ExpressionAnalyzer::Error::List* errors) {
  
}

Expression::Ptr LL1ExpressionAnalyzer::ParseBinaryExpression(
    LL1ExpressionAnalyzer::ParsingFunction child_function,
    std::initializer_list<Token::Type> token_types,
    std::initializer_list<BinaryExpression::Operator> operator_types,
    Error::List* errors) {
  auto token_type_iterator = token_types.begin();
  auto operator_types_iterator = operator_types.begin();
  auto left_expression = (this->*child_function)(errors);
  for (; token_type_iterator != token_types.end();
      ++token_type_iterator, ++operator_types_iterator) {
    auto token = lexer_->LookCurrent()->token;
    if (token->type == *token_type_iterator) {
      lexer_->MoveNext();
      auto node = std::make_shared<BinaryExpression>();
      node->left_expression = left_expression;
      node->operatorr = *operator_types_iterator;
      node->right_expression = (this->*child_function)(errors);
      return node;
    }
  }
  return left_expression;
}

Expression::Ptr LL1ExpressionAnalyzer::ParseUnaryExpression(
    LL1ExpressionAnalyzer::Error::List* errors) {
  auto left = Parse
  
}
}  // namespace vegetable_script
