#include "./ll1_expression_analyzer.h"
#include "../common/string_util.h"

namespace vegetable_script {

bool LL1ExpressionAnalyzer::Parse(
    Lexer* lexer,
    Expression::Ptr* expression,
    LL1ExpressionAnalyzer::Exception* exception) {
  return ParseExpression(lexer, expression, exception);
}

bool LL1ExpressionAnalyzer::ParseExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    LL1ExpressionAnalyzer::Exception* exception) {
  return ParsePlusExpression(lexer, expression, exception);
}

bool LL1ExpressionAnalyzer::ParsePlusExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    LL1ExpressionAnalyzer::Exception* exception) {
  return ParseBinaryExpression(
      &Self::ParseMultiplyExpression, {
          Token::Type::kOperatorPlusOrPositive,
          Token::Type::kOperatorMinusOrNegative
      }, {
          BinaryExpression::Operator::kPlus,
          BinaryExpression::Operator::kMinus
      },
      lexer,
      expression,
      exception);
}

bool LL1ExpressionAnalyzer::ParseMultiplyExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    LL1ExpressionAnalyzer::Exception* exception) {
  return ParseBinaryExpression(
      &Self::ParseUnaryExpression, {
          Token::Type::kOperatorMultiply,
          Token::Type::kOperatorDivide
      }, {
          BinaryExpression::Operator::kMultiply,
          BinaryExpression::Operator::kDivide
      },
      lexer,
      expression,
      exception);
}

bool LL1ExpressionAnalyzer::ParseBinaryExpression(
    LL1ExpressionAnalyzer::ParsingFunction child_function,
    std::initializer_list<Token::Type> token_types,
    std::initializer_list<BinaryExpression::Operator> operator_types,
    Lexer* lexer,
    Expression::Ptr* expression,
    LL1ExpressionAnalyzer::Exception* exception) {
  Expression::Ptr left_expression;
  if (!(this->*child_function)(lexer, &left_expression, exception)) {
    return false;
  }
  auto token_type_iterator = token_types.begin();
  auto operator_types_iterator = operator_types.begin();
  for (; token_type_iterator != token_types.end();
      ++token_type_iterator, ++operator_types_iterator) {
    Token token;
    if (!lexer->LookCurrentWithoutComments(&token, exception)) {
      return false;
    }
    if (token.type.contains(*token_type_iterator)) {
      lexer->MoveNext();
      auto ans_expression = std::make_shared<BinaryExpression>();
      ans_expression->left_expression = left_expression;
      ans_expression->operatorr = *operator_types_iterator;
      if (!ParseBinaryExpression(
          child_function,
          token_types,
          operator_types,
          lexer,
          &ans_expression->right_expression,
          exception)) {
        return false;
      }
      ans_expression->left_expression->parent = ans_expression;
      ans_expression->right_expression->parent = ans_expression;
      *expression = ans_expression;
      return true;
    }
  }
  *expression = left_expression;
  return true;
}

bool LL1ExpressionAnalyzer::ParseUnaryExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    LL1ExpressionAnalyzer::Exception* exception) {
  Token::Type::Type_ token_types[] = {
      Token::Type::kOperatorPlusOrPositive,
      Token::Type::kOperatorMinusOrNegative
  };
  UnaryExpression::Operator unary_operator_types[] = {
      UnaryExpression::Operator::kPositive,
      UnaryExpression::Operator::kNegative
  };
  Token token;
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  for (size_t i = 0; i < sizeof(token_types) / sizeof(token_types[0]); ++i) {
    if (token.type == token_types[i]) {
      lexer->MoveNext();
      auto ans_expression = std::make_shared<UnaryExpression>();
      ans_expression->operatorr = unary_operator_types[i];
      if (!ParseSingleExpression(lexer, &ans_expression->operand_expression,
          exception)) {
        return false;
      }
      ans_expression->operand_expression->parent = ans_expression;
      *expression = ans_expression;
      return true;
    }
  }
  return ParseSingleExpression(lexer, expression, exception);
}

bool LL1ExpressionAnalyzer::ParseSingleExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    LL1ExpressionAnalyzer::Exception* exception) {
  Token token;
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  lexer->MoveNext();
  if (token.MatchType({ Token::Type::kBracketSmall,
      Token::Type::kBracketLeft})) {
    if (!ParseExpression(lexer, expression, exception)) {
      return false;
    }
    if (!lexer->LookCurrentWithoutComments(&token, exception)) {
      return false;
    }
    if (!token.MatchType({
        Token::Type::kBracketSmall,
        Token::Type::kBracketRight})) {
      *exception = Exception {
        "right bracket is not matched", token.row, token.column
      };
      return false;
    }
    lexer->MoveNext();
    return true;
  } else if (token.type == Token::Type::kNumberFloat) {
    auto ans_expression = std::make_shared<FloatExpression>();
    ans_expression->value = xc::StringUtil::ToDouble(token.string);
    *expression = ans_expression;
    return true;
  } else if (token.type == Token::Type::kNumberInteger) {
    auto ans_expression = std::make_shared<IntegerExpression>();
    ans_expression->value = xc::StringUtil::ToInt(token.string);
    *expression = ans_expression;
    return true;
  } else if (token.type == Token::Type::kString) {
    auto ans_expression = std::make_shared<StringExpression>();
    ans_expression->value = token.string;
    *expression = ans_expression;
    return true;
  } else if (token.type == Token::Type::kIdentifier) {
    auto ans_expression = std::make_shared<IdentifierExpression>();
    ans_expression->value = token.string;
    *expression = ans_expression;
    return true;
  } else {
    *exception = Exception {
      "unknown token: \"" + token.ToString() + "\"",
      token.row, token.column
    };
    return false;
  }
}

}  // namespace vegetable_script
