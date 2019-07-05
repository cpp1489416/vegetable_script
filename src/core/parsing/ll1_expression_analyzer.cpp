#include "./ll1_expression_analyzer.h"
#include "../common/string_util.h"

namespace vegetable_script {

bool Ll1ExpressionAnalyzer::Parse(
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  return ParseExpression(lexer, expression, exception);
}

bool Ll1ExpressionAnalyzer::ParseExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  return ParseCommaExpression(lexer, expression, exception);
}

bool Ll1ExpressionAnalyzer::ParseCommaExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  return ParseBinaryExpressionFromLeftToRight(
      &Self::ParseAssignExpression, {
          Token::Type::kOperatorComma
      }, {
          BinaryExpression::Operator::kComma
      },
      lexer,
      expression,
      exception);
}

bool Ll1ExpressionAnalyzer::ParseAssignExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  return ParseBinaryExpressionFromRightToLeft(
      &Self::ParseOrExpression, {
          Token::Type::kOperatorAssign
      }, {
          BinaryExpression::Operator::kAssign
      },
      lexer,
      expression,
      exception);
}

bool Ll1ExpressionAnalyzer::ParseOrExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  return ParseBinaryExpressionFromRightToLeft(
      &Self::ParseAndExpression, {
          Token::Type::kOperatorOr
      }, {
          BinaryExpression::Operator::kOr
      },
      lexer,
      expression,
      exception);
}

bool Ll1ExpressionAnalyzer::ParseAndExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  return ParseBinaryExpressionFromLeftToRight(
      &Self::ParseEqualExpression, {
          Token::Type::kOperatorAnd,
      }, {
          BinaryExpression::Operator::kAnd,
      },
      lexer,
      expression,
      exception);
}

bool Ll1ExpressionAnalyzer::ParseEqualExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  return ParseBinaryExpressionFromLeftToRight(
      &Self::ParsePlusExpression, {
          Token::Type::kOperatorEqual,
          Token::Type::kOperatorNotEqual,
          Token::Type::kOperatorGreater,
          Token::Type::kOperatorGreaterEqual,
          Token::Type::kOperatorLesser,
          Token::Type::kOperatorLesserEqual,
      }, {
          BinaryExpression::Operator::kEqual,
          BinaryExpression::Operator::kNotEqual,
          BinaryExpression::Operator::kGreater,
          BinaryExpression::Operator::kGreaterEqual,
          BinaryExpression::Operator::kLesser,
          BinaryExpression::Operator::kLesserEqual,
      },
      lexer,
      expression,
      exception);
}

bool Ll1ExpressionAnalyzer::ParsePlusExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  return ParseBinaryExpressionFromLeftToRight(
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

bool Ll1ExpressionAnalyzer::ParseMultiplyExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  return ParseBinaryExpressionFromLeftToRight(
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

bool Ll1ExpressionAnalyzer::ParseBinaryExpressionFromLeftToRight(
    Ll1ExpressionAnalyzer::ParsingFunction child_function,
    std::initializer_list<Token::Type> token_types,
    std::initializer_list<BinaryExpression::Operator> operator_types,
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  Expression::Ptr ans_expression;
  if (!(this->*child_function)(lexer, &ans_expression, exception)) {
    return false;
  }
  while (true) {
    auto token_type_iterator = token_types.begin();
    auto operator_types_iterator = operator_types.begin();
    Token token;
    bool found = false;
    for (; token_type_iterator != token_types.end();
        ++token_type_iterator, ++operator_types_iterator) {
      if (!lexer->LookCurrentWithoutComments(&token, exception)) {
        return false;
      }
      if (token.type.contains(*token_type_iterator)) {
        lexer->MoveNext();
        found = true;
        auto tmp_expression = std::make_shared<BinaryExpression>();
        tmp_expression->left_expression = ans_expression;
        tmp_expression->operatorr = *operator_types_iterator;
        if (!(this->*child_function)(lexer, &tmp_expression->right_expression,
            exception)) {
          return false;
        }
        tmp_expression->left_expression->parent = tmp_expression;
        tmp_expression->right_expression->parent = tmp_expression;
        ans_expression = tmp_expression;
      }
    }
    if (!found) {
      break;
    }
  }
  *expression = ans_expression;
  return true;
}

bool Ll1ExpressionAnalyzer::ParseBinaryExpressionFromRightToLeft(
    ParsingFunction child_function,
    std::initializer_list<Token::Type> token_types,
    std::initializer_list<BinaryExpression::Operator> operator_types,
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
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
      if (!ParseBinaryExpressionFromRightToLeft(
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

bool Ll1ExpressionAnalyzer::ParseUnaryExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  Token::Type::Type_ token_types[] = {
      Token::Type::kOperatorPlusOrPositive,
      Token::Type::kOperatorMinusOrNegative,
      Token::Type::kOperatorNot
  };
  UnaryExpression::Operator unary_operator_types[] = {
      UnaryExpression::Operator::kPositive,
      UnaryExpression::Operator::kNegative,
      UnaryExpression::Operator::kNot,
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

bool Ll1ExpressionAnalyzer::ParseSingleExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  Token token;
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (token.MatchType({ Token::Type::kBracketSmall,
      Token::Type::kBracketLeft})) {
    lexer->MoveNext();
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
    lexer->MoveNext();
    auto ans_expression = std::make_shared<FloatExpression>();
    ans_expression->value = xc::StringUtil::ToDouble(token.string);
    *expression = ans_expression;
    return true;
  } else if (token.type == Token::Type::kNumberInteger) {
    lexer->MoveNext();
    auto ans_expression = std::make_shared<IntegerExpression>();
    ans_expression->value = xc::StringUtil::ToInt(token.string);
    *expression = ans_expression;
    return true;
  } else if (token.type == Token::Type::kString) {
    lexer->MoveNext();
    auto ans_expression = std::make_shared<StringExpression>();
    ans_expression->value = token.string;
    *expression = ans_expression;
    return true;
  } else if (token.type == Token::Type::kIdentifier) {
    Token next_token;
    if (!lexer->LookAheadWithoutComments(&next_token, exception)) {
      return false;
    }
    if (next_token.MatchType({Token::Type::kBracketSmall,
        Token::Type::kBracketLeft})) {
      return ParseFunctionInvokeExpression(lexer, expression, exception);
    } else {
      lexer->MoveNext();
      auto ans_expression = std::make_shared<IdentifierExpression>();
      ans_expression->value = token.string;
      *expression = ans_expression;
      return true;
    }
  } else {
    *exception = {
      "unknown token: \"" + token.string + "\"",
      token.row, token.column
    };
    return false;
  }
}

bool Ll1ExpressionAnalyzer::ParseFunctionInvokeExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    Ll1ExpressionAnalyzer::Exception* exception) {
  auto ans_expression = std::make_shared<FunctionInvokeExpression>();
  Token token;
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  ans_expression->function_name = token.string;
  lexer->MoveNext();
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (!token.MatchType({Token::Type::kBracketSmall,
      Token::Type::kBracketLeft})) {
    *exception = {
      "a function invoke expression must have a \"(\" after function name",
      token.row, token.column
    };
    return false;
  }
  lexer->MoveNext();
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (token.MatchType({Token::Type::kBracketSmall,
      Token::Type::kBracketRight})) {
    lexer->MoveNext();
    *expression = ans_expression;
    return true;
  }
  while (true) {
    if (!lexer->LookCurrentWithoutComments(&token, exception)) {
      return false;
    }
    Expression::Ptr argument;
    if (!ParseAssignExpression(lexer, &argument, exception)) {
      return false;
    }
    ans_expression->arguments.push_back(argument);
    if (!lexer->LookCurrentWithoutComments(&token, exception)) {
      return false;
    }
    if (token.MatchType({Token::Type::kBracketSmall,
        Token::Type::kBracketRight})) {
      lexer->MoveNext();
      break;
    }
    if (!token.MatchType({Token::Type::kOperatorComma})) {
      *exception = Exception {
        "each expression must be splitted with a \",\"",
        token.row, token.column
      };
      return false;
    }
    lexer->MoveNext();
  }

  *expression = ans_expression;
  return true;
}

}  // namespace vegetable_script
