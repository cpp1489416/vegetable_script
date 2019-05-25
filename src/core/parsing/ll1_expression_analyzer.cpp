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
  return ParseCommaExpression(lexer, expression, exception);
}

bool LL1ExpressionAnalyzer::ParseCommaExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    LL1ExpressionAnalyzer::Exception* exception) {
  return ParseBinaryExpression(
      &Self::ParsePlusExpression, {
          Token::Type::kOperatorComma
      }, {
          BinaryExpression::Operator::kComma
      },
      lexer,
      expression,
      exception);
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
    *exception = Exception {
      "unknown token: \"" + token.string + "\"",
      token.row, token.column
    };
    return false;
  }
}

bool LL1ExpressionAnalyzer::ParseFunctionInvokeExpression(
    Lexer* lexer,
    Expression::Ptr* expression,
    LL1ExpressionAnalyzer::Exception* exception) {
  auto ans_expression = std::make_shared<FunctionInvokeExpression>();
  Token token;
  if (!lexer->LookCurrent(&token, exception)) {
    return false;
  }
  ans_expression->function_name = token.string;
  lexer->MoveNext();
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (!token.MatchType({Token::Type::kBracketSmall,
      Token::Type::kBracketLeft})) {
    *exception = Exception {
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
    if (!ParsePlusExpression(lexer, &argument, exception)) {
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
