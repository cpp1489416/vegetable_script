
#include "./lexer.h"
#include <cstring>
#include <iostream>

namespace {

bool IsIdentifier(char c) {
  return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == '_';
}

bool IsNumber(char c) {
  return c >= '0' && c <= '9';
}

bool IsSubIdentifier(char c) {
  return IsIdentifier(c) || IsNumber(c);
}

bool IsBlank(char c) {
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

}  // namespace

namespace vegetable_script {

Lexer::Lexer(SourceProvider::Ptr source_provider, bool on_error_skip):
  source_provider_(source_provider), on_error_skip_(on_error_skip) {
}

bool Lexer::HasNext() {
  if (tokens_.size() > 1) {
    return true;
  }
  if (tokens_.size() == 0) {
    return true;
  }
  if (tokens_.front()->type == Token::Type::kEnd) {
    return false;
  }
  return true;
}

void Lexer::MoveNext() {
  if (tokens_.size() == 0) {
    return;
  }
  if (tokens_.front()->type == Token::Type::kEnd) {
    return;
  }
  tokens_.pop_front();
}

Token::Ptr Lexer::LookCurrent(Lexer::Error::List* errors) {
  if (tokens_.size() == 0) {
    Epoch(errors);
    return tokens_.front();
  } else {
    return tokens_.front();
  }
}

Token::Ptr Lexer::LookCurrentWithoutComments(Lexer::Error::List* errors) {
  if (tokens_.size() == 0) {
    Epoch(errors);
  }
  while (tokens_.front()->type == Token::Type::kComment) {
    tokens_.pop_front();
    if (tokens_.size() == 0) {
      Epoch(errors);
    }
  }
  return tokens_.front();
}

Token::Ptr Lexer::LookAhead(Lexer::Error::List* errors) {
  return LookAhead(1, errors);
}

Token::Ptr Lexer::LookAheadWithoutComments(Lexer::Error::List* errors) {
  return LookAheadWithoutComments(1, errors);
}

Token::Ptr Lexer::LookAhead(int more, Lexer::Error::List* errors) {
  if (more <= 0) {
    return LookCurrent(errors);
  }
  if (tokens_.size() >= more + 1) {
    return tokens_[more];
  }
  size_t times = 1 + more - tokens_.size();
  while (times--) {
    Epoch(errors);
  }
  return tokens_.back();
}

Token::Ptr Lexer::LookAheadWithoutComments(int more,
    Lexer::Error::List* errors) {
  int index = 0;
  ++more;
  while (true) {
    if (tokens_.size() <= index) {
      Epoch(errors);
    }
    auto token = tokens_[index];
    ++index;
    if (token->type == Token::Type::kComment) {
      continue;
    } else if (token->type == Token::Type::kEnd) {
      return token;
    } else {
      --more;
      if (more == 0) {
        return token;
      }
    }
  }
}

void Lexer::Epoch(Lexer::Error::List* errors) {
  if (tokens_.size() != 0 &&
      tokens_.front()->type == Token::Type::kEnd) {
    return;
  }
  SkipBlanks();
  if (!source_provider_->HasNext()) {
    PushBackToken("", Token::Type::kEnd,
        source_provider_->LookRow(), source_provider_->LookColumn());
    return;
  }
  Epoch0(errors);
}

void Lexer::Epoch0(Lexer::Error::List* errors) {
  int row = source_provider_->LookRow();
  int column = source_provider_->LookColumn();
  std::string string;
  char c = source_provider_->LookCurrent();
  source_provider_->MoveNext();
  string += c;
  int status;
  switch (c) {
    case ';':
      PushBackToken(";", Token::Type::kSemicolon, row, column);
      return;
    case '(':
      PushBackToken(
          "(",
          Token::Type() <<
              Token::Type::kBracket <<
              Token::Type::kBracketSmall <<
              Token::Type::kBracketLeft,
          row,
          column);
      return;
    case ')':
      PushBackToken(
          ")",
          Token::Type() <<
              Token::Type::kBracket <<
              Token::Type::kBracketSmall <<
              Token::Type::kBracketRight,
          row,
          column);
      return;
    case '[':
      PushBackToken(
          "[",
          Token::Type() <<
              Token::Type::kBracket <<
              Token::Type::kBracketMedium <<
              Token::Type::kBracketLeft,
          row,
          column);
        return;
    case ']':
      PushBackToken(
          "]",
          Token::Type() <<
              Token::Type::kBracket <<
              Token::Type::kBracketMedium <<
              Token::Type::kBracketRight,
          row,
          column);
      return;
    case '{':
      PushBackToken(
          "{",
          Token::Type() <<
              Token::Type::kBracket <<
              Token::Type::kBracketBig <<
            Token::Type::kBracketLeft,
          row,
          column);
      return;
    case '}':
      PushBackToken(
          "}",
          Token::Type() <<
              Token::Type::kBracket <<
              Token::Type::kBracketBig <<
              Token::Type::kBracketRight,
          row,
          column);
      return;
    case '*':
      PushBackToken(
          "*",
          Token::Type() <<
              Token::Type::kOperator <<
              Token::Type::kOperatorMultiply,
          row,
          column);
      return;
    case '<':
      PushBackToken(
          "<",
          Token::Type() <<
              Token::Type::kOperator <<
              Token::Type::kOperatorLesser,
          row,
          column);
      return;
    case '>':
      PushBackToken(
          ">",
          Token::Type() <<
              Token::Type::kOperator <<
              Token::Type::kOperatorGreater,
          row,
          column);
      return;
    case '\0':
      PushBackToken("", Token::Type::kEnd, row, column);
      return;
    case '.':
      status = 15;
      break;
    case '+':
    case '-':
      status = 4;
      break;
    case '&':
      status = 5;
      break;
    case '|':
      status = 7;
      break;
    case '/':
      status = 9;
      break;
    case '"':
      status = 13;
      string.pop_back();
      break;
    default: {
      if (IsIdentifier(c)) {
        status = 1;
      } else if (IsNumber(c)) {
        status = 2;
      } else {
        PushBackError(
            std::string() + "invalid char'" + c + "'",
            row, column, errors);
      }
      break;
    }
  }
  EpochElse(status, row, column, &string, errors);
}

void Lexer::EpochElse(int status, int row, int column,
    std::string* string, Lexer::Error::List* errors) {
  auto push_back_comment_not_end_error = [&]() {
    PushBackError(
        "a string must have end double quotation mark "
        "or in a single line",
        source_provider_->LookRow(),
        source_provider_->LookColumn(),
        errors);
  };
  auto push_back_string_not_end_error = [&]() {
    PushBackError(
        "a multi-line comment must have its end mark '*/'",
        source_provider_->LookRow(),
        source_provider_->LookColumn(),
        errors);
  };
  while (true) {
    char c = source_provider_->LookCurrent();
    switch (status) {
      case 0: {
        break;
      }
      case 1: {
        char c = source_provider_->LookCurrent();
        if (IsSubIdentifier(c)) {
          *string += c;
          source_provider_->MoveNext();
        } else {
          PushBackToken(*string, Token::Type::kIdentifier, row, column);
          return;
        }
        break;
      }
      case 2: {
        char c = source_provider_->LookCurrent();
        if (IsNumber(c)) {
          *string += c;
          source_provider_->MoveNext();
        } else if (c == '.') {
          *string += c;
          source_provider_->MoveNext();
          status = 3;
        } else {
          PushBackToken(
              *string,
              Token::Type() <<
                  Token::Type::kNumber <<
                  Token::Type::kNumberInteger,
              row,
              column);
          return;
        }
        break;
      }
      case 3: {
        char c = source_provider_->LookCurrent();
        if (IsNumber(c)) {
          *string += c;
          source_provider_->MoveNext();
        } else {
          PushBackToken(
              *string,
              Token::Type() <<
                  Token::Type::kNumber <<
                  Token::Type::kNumberFloat,
              row,
              column);
          return;
        }
        break;
      }
      case 4: {
        char c = source_provider_->LookCurrent();
        if (IsNumber(c)) {
          *string += c;
          source_provider_->MoveNext();
          status = 2;
        } else if (c == '.') {
          *string += c;
          source_provider_->MoveNext();
          status = 3;
        } else {
          Token::Type type = Token::Type::kOperator;
          if (*string == "+") {
            type << Token::Type::kOperatorPlusOrPositive;
          } else {
            type << Token::Type::kOperatorMinusOrNegative;
          }
          PushBackToken(
              *string,
              type,
              row,
              column);
          return;
        }
        break;
      }
      case 5: {
        char c = source_provider_->LookCurrent();
        if (c == '&') {
          *string += c;
          source_provider_->MoveNext();
          PushBackToken(
              *string,
              Token::Type() <<
                  Token::Type::kOperator <<
                  Token::Type::kOperatorAnd,
              row,
              column);
          return;
        } else {
          PushBackToken(
              *string,
              Token::Type() <<
                  Token::Type::kOperator <<
                  Token::Type::kOperatorAnd,
              row,
              column);
          return;
        }
        break;
      }
      case 7: {
        char c = source_provider_->LookCurrent();
        if (c == '|') {
          *string += c;
          source_provider_->MoveNext();
          PushBackToken(
              *string,
              Token::Type() <<
                  Token::Type::kOperator <<
                  Token::Type::kOperatorOr,
              row,
              column);
          return;
        } else {
          source_provider_->MoveNext();
          PushBackToken(
              *string,
              Token::Type() <<
                  Token::Type::kOperator <<
                  Token::Type::kOperatorOr,
              row,
              column);
          return;
        }
        break;
      }
      case 9: {
        char c = source_provider_->LookCurrent();
        if (c == '/') {
          *string += c;
          source_provider_->MoveNext();
          status = 10;
        } else if (c == '*') {
          *string += c;
          source_provider_->MoveNext();
          status = 11;
        } else {
          PushBackToken(
              *string,
              Token::Type() <<
                  Token::Type::kOperator <<
                  Token::Type::kOperatorDivide,
              row,
              column);
          return;
        }
        break;
      }
      case 10: {
        char c = source_provider_->LookCurrent();
        if (c == '\n' || c == '\0') {
          source_provider_->MoveNext();
          PushBackToken(
              *string,
              Token::Type::kComment,
              row,
              column);
          return;
        } else {
          *string += c;
          source_provider_->MoveNext();
        }
        break;
      }
      case 11: {
        char c = source_provider_->LookCurrent();
        if (c == '*') {
          *string += c;
          source_provider_->MoveNext();
          status = 12;
        } else if (c != '\0') {
          *string += c;
          source_provider_->MoveNext();
        } else {
          push_back_comment_not_end_error();
          return;
        }
        break;
      }
      case 12: {
        char c = source_provider_->LookCurrent();
        if (c == '/') {
          *string += c;
          source_provider_->MoveNext();
          PushBackToken(
              *string,
              Token::Type::kComment,
              row,
              column);
          return;
        } else if (c != '\0') {
          *string += c;
          source_provider_->MoveNext();
          status = 11;
        } else {
          push_back_string_not_end_error();
          return;
        }
        break;
      }
      case 13: {
        char c = source_provider_->LookCurrent();
        if (c == '"') {
          source_provider_->MoveNext();
          PushBackToken(
              *string,
              Token::Type::kString,
              row,
              column);
          return;
        } else if (c == '\\') {
          source_provider_->MoveNext();
          status = 14;
        } else if (c != '\n' && c != '\0') {
          *string += c;
          source_provider_->MoveNext();
        } else {
          push_back_comment_not_end_error();
          return;
        }
        break;
      }
      case 14: {
        char c = source_provider_->LookCurrent();
        if (c != '\0') {
          *string += c;
          source_provider_->MoveNext();
          status = 13;
        } else {
          push_back_string_not_end_error();
          return;
        }
        break;
      }
      case 15: {
        char c = source_provider_->LookCurrent();
        if (IsNumber(c)) {
          *string += c;
          source_provider_->MoveNext();
          status = 3;
        } else {
          push_back_comment_not_end_error();
          return;
        }
      }
      default: {
        break;
      }
    }
  }
}  // NOLINT

void Lexer::SkipBlanks() {
  while (source_provider_->HasNext()) {
    char c = source_provider_->LookCurrent();
    if (IsBlank(c)) {
      source_provider_->MoveNext();
    } else {
      return;
    }
  }
}

void Lexer::PushBackToken(const std::string& string,
    const Token::Type& type, int row, int column) {
  tokens_.push_back(std::make_shared<Token>(string, type, row, column));
}

void Lexer::PushBackError(const std::string& status, int row, int column,
    Lexer::Error::List* errors) {
  errors->push_back(Error {
    status, row, column});
  if (on_error_skip()) {
    Epoch(errors);
  } else {
    tokens_.push_back(
        std::make_shared<Token>("", Token::Type::kEnd, row, column));
  }
}

}  // namespace vegetable_script
