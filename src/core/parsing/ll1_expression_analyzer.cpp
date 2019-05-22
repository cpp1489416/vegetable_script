#include "./ll1_expression_analyzer.h"
#include "../common/string_util.h"

namespace vegetable_script {

LL1ExpressionAnalyzer::LL1ExpressionAnalyzer(Lexer::Ptr lexer) :
    lexer_(lexer) {
}

LL1ExpressionAnalyzer::Result::Ptr LL1ExpressionAnalyzer::Parse() {
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
  return ParseBinaryExpression(
      &Self::ParseUnaryExpression, {
          Token::Type::kOperatorPlus,
          Token::Type::kOperatorMinus
      }, {
          BinaryExpression::Operator::kPlus,
          BinaryExpression::Operator::kMinus
      },
      errors);
}

Expression::Ptr LL1ExpressionAnalyzer::ParseMultiplyExpression(
    LL1ExpressionAnalyzer::Error::List* errors) {
  return ParseBinaryExpression(
      &Self::ParseMultiplyExpression, {
          Token::Type::kOperatorMultiply,
          Token::Type::kOperatorDivide
      }, {
          BinaryExpression::Operator::kMultiply,
          BinaryExpression::Operator::kDivide
      },
      errors);
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
      auto expression = std::make_shared<BinaryExpression>();
      expression->left_expression = left_expression;
      expression->left_expression->parent = expression;
      expression->operatorr = *operator_types_iterator;
      expression->right_expression = (this->*child_function)(errors);
      expression->right_expression->parent = expression;
      return expression;
    }
  }
  return left_expression;
}

Expression::Ptr LL1ExpressionAnalyzer::ParseUnaryExpression(
    LL1ExpressionAnalyzer::Error::List* errors) {
  Token::Type::Type_ token_types[] = {
      Token::Type::kOperatorPositive,
      Token::Type::kOperatorNegative
  };
  UnaryExpression::Operator unary_operator_types[] = {
      UnaryExpression::Operator::kPositive,
      UnaryExpression::Operator::kNegative
  };
  auto token = lexer_->LookCurrent()->token;
  for (size_t i = 0; i < sizeof(token_types) / sizeof(token_types[0]); ++i) {
    if (token->type == token_types[i]) {
      lexer_->MoveNext();
      auto expression = std::make_shared<UnaryExpression>();
      expression->operatorr = unary_operator_types[i];
      expression->child_expression = ParseSingleExpression(errors);
      expression->child_expression->parent = expression;
      return expression;
    }
  }
  return ParseUnaryExpression(errors);
}


Expression::Ptr LL1ExpressionAnalyzer::ParseSingleExpression(
    LL1ExpressionAnalyzer::Error::List* errors) {
  auto token = lexer_->LookCurrent()->token;
  lexer_->MoveNext();
  if (token->MatchType({
      Token::Type::kBracketSmall,
      Token::Type::kBracketLeft})) {
    auto expression = ParseExpression(errors);
    token = lexer_->LookCurrent()->token;
    if (!token->MatchType({
        Token::Type::kBracketSmall,
        Token::Type::kBracketRight})) {
      errors->push_back(std::make_shared<Error>(
          "right bracket not matched", -1, -1));
    }
    lexer_->MoveNext();
    return expression;
  } else if (token->type == Token::Type::kNumberFloat) {
    auto expression = std::make_shared<FloatExpression>();
    expression->value = xc::StringUtil::ToDouble(token->string);
    return expression;
  } else if (token->type == Token::Type::kNumberInteger) {
    auto expression = std::make_shared<FloatExpression>();
    expression->value = xc::StringUtil::ToInt(token->string);
    return expression;
  } else if (token->type == Token::Type::kString) {
    auto expression = std::make_shared<StringExpression>();
    expression->value = token->string.substr(1, token->string.length() - 2);
    return expression;
  } else if (token->type == Token::Type::kIdentifier) {
    auto next_token = lexer_->LookCurrent()->token;
    auto expression = std::make_shared<IdentifierExpression>();
    expression->value = token->value;
    return expression;
  } else {
    errors.push_back(std::make_shared<Error>(
        "unknown token: " + token->ToString(), -1, -1));
    return nullptr;
  }
}

}  // namespace vegetable_script
