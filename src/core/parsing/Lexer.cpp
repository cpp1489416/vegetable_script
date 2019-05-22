
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

Lexer::Lexer(SourceProvider::Ptr source_provider):
  source_provider_(source_provider) {
}

bool Lexer::HasNext() {
  if (results_.size() > 1) {
    return true;
  }
  if (results_.size() == 0) {
    Epoch();
  }
  if (results_.front()->token->type == Token::Type::kEnd) {
    return false;
  }
  return true;
}

Lexer::Result::Ptr Lexer::MoveNext() {
  if (results_.size() == 0) {
    LookCurrent();
  }
  results_.pop_front();
  Epoch();
  return LookCurrent();
}

Lexer::Result::Ptr Lexer::LookCurrent() {
  if (results_.size() == 0) {
    Epoch();
	std::cout << "start'";
  }
  return results_.front();
}

Lexer::Result::Ptr Lexer::LookAhead(int more) {
  if (more <= 0) {
    return LookCurrent();
  }
  if (results_.size() >= more + 1) {
    return results_[more];
  }
  size_t times = 1 + more - results_.size();
  while (times--) {
    Epoch();
  }
  return results_.back();
}

void Lexer::Epoch() {
  if (results_.size() != 0 &&
      results_.front()->token->type == Token::Type::kEnd) {
    return;
  }
  SkipBlanks();
  if (!source_provider_->HasNext()) {
    PushBackToken("", Token::Type::kEnd,
        source_provider_->LookRow(), source_provider_->LookColumn());
    return;
  }
  Epoch0();
}

void Lexer::Epoch0() {
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
              Token::Type::kOperatorLess,
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
    case '.':
      status = 3;
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
      break;
    default: {
      if (IsIdentifier(c)) {
        status = 1;
      } else if (IsNumber(c)) {
        status = 2;
      } else {
        PushBackError(row, column);
        return;
      }
      break;
    }
  }
  EpochElse(status, &string, row, column);
}

void Lexer::EpochElse(int status, std::string* string, int row, int column) {
  while (true) {
    char c = source_provider_->LookCurrent();
    if (c == '\0') {
      PushBackError(row, column);
      return;
    }
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
            type << Token::Type::kOperatorPlus;
          } else {
            type << Token::Type::kOperatorMinus;
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
        if (c == '\n') {
          *string += c;
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
        } else {
          *string += c;
          source_provider_->MoveNext();
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
        } else {
          *string += c;
          source_provider_->MoveNext();
          status = 11;
        }
        break;
      }
      case 13: {
        char c = source_provider_->LookCurrent();
        if (c == '"') {
          *string += c;
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
        } else {
          *string += c;
          source_provider_->MoveNext();
        }
        break;
      }
      case 14: {
        char c = source_provider_->LookCurrent();
        *string += c;
        source_provider_->MoveNext();
        status = 13;
        break;
      }
      default: {
        break;
      }
    }
  }
}

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
  results_.push_back(Result::Ptr(new Result {
      Token::Ptr(new Token(string, type, row, column)),
      nullptr
  }));
}

void Lexer::PushBackError(int row, int column) {
  results_.push_back(Result::Ptr(new Result {
      Token::Ptr(new Token("", Token::Type::kError, row, column)),
      Result::Error::Ptr(new Result::Error {
          row, column
      })
  }));
}

}  // namespace vegetable_script
