#include "./ll1_expression_analyzer.h"
#include "../common/string_util.h"

namespace vegetable_script {

LL1ExpressionAnalyzer::LL1ExpressionAnalyzer(bool on_error_skip)
    : on_error_skip_(on_error_skip) { }

LL1ExpressionAnalyzer::Result::Ptr LL1ExpressionAnalyzer::Parse(Lexer* lexer) {
  Error::List errors;
  auto expression = ParseExpression(lexer, &errors);
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
    Lexer* lexer,
    LL1ExpressionAnalyzer::Error::List* errors) {
  return ParsePlusExpression(lexer, errors);
}

Expression::Ptr LL1ExpressionAnalyzer::ParsePlusExpression(
    Lexer* lexer,
    LL1ExpressionAnalyzer::Error::List* errors) {
  return ParseBinaryExpression(
      &Self::ParseMultiplyExpression, {
          Token::Type::kOperatorPlusOrPositive,
          Token::Type::kOperatorMinusOrNegative
      }, {
          BinaryExpression::Operator::kPlus,
          BinaryExpression::Operator::kMinus
      },
      lexer,
      errors);
}

Expression::Ptr LL1ExpressionAnalyzer::ParseMultiplyExpression(
    Lexer* lexer,
    LL1ExpressionAnalyzer::Error::List* errors) {
  return ParseBinaryExpression(
      &Self::ParseUnaryExpression, {
          Token::Type::kOperatorMultiply,
          Token::Type::kOperatorDivide
      }, {
          BinaryExpression::Operator::kMultiply,
          BinaryExpression::Operator::kDivide
      },
      lexer,
      errors);
}

Expression::Ptr LL1ExpressionAnalyzer::ParseBinaryExpression(
    LL1ExpressionAnalyzer::ParsingFunction child_function,
    std::initializer_list<Token::Type> token_types,
    std::initializer_list<BinaryExpression::Operator> operator_types,
    Lexer* lexer,
    Error::List* errors) {
  auto token_type_iterator = token_types.begin();
  auto operator_types_iterator = operator_types.begin();
  auto left_expression = (this->*child_function)(lexer, errors);
  std::cout << "size of token_types: " << token_types.size() << std::endl;
  for (; token_type_iterator != token_types.end();
      ++token_type_iterator, ++operator_types_iterator) {
    std::cout << token_type_iterator->ToString() << std::endl;
    auto token = lexer->LookCurrent()->token;
    if (token->type.contains(*token_type_iterator)) {
      std::cout << "found" << token->type.ToString() << "\n";
      lexer->MoveNext();
      auto expression = std::make_shared<BinaryExpression>();
      expression->left_expression = left_expression;
      expression->left_expression->parent = expression;
      expression->operatorr = *operator_types_iterator;
      expression->right_expression = (this->*child_function)(lexer, errors);
      expression->right_expression->parent = expression;
      return expression;
    }
  }
  return left_expression;
}

Expression::Ptr LL1ExpressionAnalyzer::ParseUnaryExpression(
    Lexer* lexer,
    LL1ExpressionAnalyzer::Error::List* errors) {
  Token::Type::Type_ token_types[] = {
      Token::Type::kOperatorPlusOrPositive,
      Token::Type::kOperatorMinusOrNegative
  };
  UnaryExpression::Operator unary_operator_types[] = {
      UnaryExpression::Operator::kPositive,
      UnaryExpression::Operator::kNegative
  };
  auto token = lexer->LookCurrent()->token;
  for (size_t i = 0; i < sizeof(token_types) / sizeof(token_types[0]); ++i) {
    if (token->type == token_types[i]) {
      lexer->MoveNext();
      auto expression = std::make_shared<UnaryExpression>();
      expression->operatorr = unary_operator_types[i];
      expression->child_expression = ParseSingleExpression(lexer, errors);
      expression->child_expression->parent = expression;
      return expression;
    }
  }
  return ParseSingleExpression(lexer, errors);
}

Expression::Ptr LL1ExpressionAnalyzer::ParseSingleExpression(
    Lexer* lexer,
    LL1ExpressionAnalyzer::Error::List* errors) {
  auto token = lexer->LookCurrent()->token;
  lexer->MoveNext();
  if (token->MatchType({
      Token::Type::kBracketSmall,
      Token::Type::kBracketLeft})) {
    auto expression = ParseExpression(lexer, errors);
    token = lexer->LookCurrent()->token;
    if (!token->MatchType({
        Token::Type::kBracketSmall,
        Token::Type::kBracketRight})) {
      errors->push_back(std::make_shared<Error>(
          "right bracket not matched", -1, -1));
    }
    lexer->MoveNext();
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
    auto next_token = lexer->LookCurrent()->token;
    auto expression = std::make_shared<IdentifierExpression>();
    expression->value = token->string;
    return expression;
  } else {
    errors->push_back(std::make_shared<Error>(
        "unknown token: " + token->ToString(), -1, -1));
    return nullptr;
  }
}

}  // namespace vegetable_script
