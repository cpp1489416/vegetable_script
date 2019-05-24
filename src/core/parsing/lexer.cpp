
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
  if (tokens_.size() == 0) {
    return true;
  }
  if (tokens_.front().type == Token::Type::kEnd) {
    return false;
  }
  return true;
}

void Lexer::MoveNext() {
  if (tokens_.size() == 0) {
    return;
  }
  if (tokens_.front().type == Token::Type::kEnd) {
    return;
  }
  tokens_.pop_front();
}

bool Lexer::LookCurrent(Token* token, ParsingException* exception) {
  if (tokens_.size() == 0 && !Epoch(exception)) {
    return false;
  }
  *token = tokens_.front();
  return true;
}

bool Lexer::LookCurrentWithoutComments(
    Token* token, ParsingException* exception) {
  if (tokens_.size() == 0 && !Epoch(exception)) {
    return false;
  }
  while (tokens_.front().type == Token::Type::kComment) {
    tokens_.pop_front();
    if (tokens_.size() == 0 && !Epoch(exception)) {
      return false;
    }
  }
  *token = tokens_.front();
  return true;
}

bool Lexer::LookAhead(Token* token, ParsingException* exception) {
  return LookAhead(1, token, exception);
}

bool Lexer::LookAheadWithoutComments(
    Token* token, ParsingException* exception) {
  return LookAheadWithoutComments(1, token, exception);
}

bool Lexer::LookAhead(size_t more,
    Token* token, ParsingException* exception) {
  if (more == 0) {
    return LookCurrent(token, exception);
  }
  if (tokens_.size() >= more + 1) {
    *token = tokens_[more];
    return true;
  }
  size_t times = 1 + more - tokens_.size();
  while (times--) {
    if (!Epoch(exception)) {
      return false;
    }
  }
  *token = tokens_.back();
  return true;
}

bool Lexer::LookAheadWithoutComments(size_t more,
    Token* token, ParsingException* exception) {
  size_t index = 0;
  ++more;
  while (true) {
    if (tokens_.size() <= index && !Epoch(exception)) {
      return false;
    }
    auto cur_token = tokens_[index];
    ++index;
    if (cur_token.type == Token::Type::kComment) {
      continue;
    } else if (cur_token.type == Token::Type::kEnd) {
      *token = cur_token;
      return true;
    } else {
      --more;
      if (more == 0) {
        *token = cur_token;
        return true;
      }
    }
  }
}

bool Lexer::Epoch(ParsingException* exception) {
  if (tokens_.size() != 0 &&
      tokens_.back().type == Token::Type::kEnd) {
    return true;
  }
  SkipBlanks();
  if (!source_provider_->HasNext()) {
    tokens_.push_back(Token {
        std::string(""), Token::Type::kEnd,
        source_provider_->LookRow(), source_provider_->LookColumn()
    });
    return true;
  }
  return Epoch0(exception);
}

bool Lexer::Epoch0(ParsingException* exception) {
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
      return true;
    case '(':
      PushBackToken(
          "(",
          Token::Type() <<
              Token::Type::kBracket <<
              Token::Type::kBracketSmall <<
              Token::Type::kBracketLeft,
          row,
          column);
      return true;
    case ')':
      PushBackToken(
          ")",
          Token::Type() <<
              Token::Type::kBracket <<
              Token::Type::kBracketSmall <<
              Token::Type::kBracketRight,
          row,
          column);
      return true;
    case '[':
      PushBackToken(
          "[",
          Token::Type() <<
              Token::Type::kBracket <<
              Token::Type::kBracketMedium <<
              Token::Type::kBracketLeft,
          row,
          column);
        return true;
    case ']':
      PushBackToken(
          "]",
          Token::Type() <<
              Token::Type::kBracket <<
              Token::Type::kBracketMedium <<
              Token::Type::kBracketRight,
          row,
          column);
      return true;
    case '{':
      PushBackToken(
          "{",
          Token::Type() <<
              Token::Type::kBracket <<
              Token::Type::kBracketBig <<
            Token::Type::kBracketLeft,
          row,
          column);
      return true;
    case '}':
      PushBackToken(
          "}",
          Token::Type() <<
              Token::Type::kBracket <<
              Token::Type::kBracketBig <<
              Token::Type::kBracketRight,
          row,
          column);
      return true;
    case '*':
      PushBackToken(
          "*",
          Token::Type() <<
              Token::Type::kOperator <<
              Token::Type::kOperatorMultiply,
          row,
          column);
      return true;
    case '<':
      PushBackToken(
          "<",
          Token::Type() <<
              Token::Type::kOperator <<
              Token::Type::kOperatorLesser,
          row,
          column);
      return true;
    case '>':
      PushBackToken(
          ">",
          Token::Type() <<
              Token::Type::kOperator <<
              Token::Type::kOperatorGreater,
          row,
          column);
      return true;
    case '\0':
      PushBackToken("", Token::Type::kEnd, row, column);
      return true;
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
        if (exception != nullptr) {
          *exception = ParsingException {
              std::string() + "invalid char'" + c + "'",
              row,
              column
          };
          return false;
        }
      }
      break;
    }
  }
  return EpochElse(status, row, column, &string, exception);
}

bool Lexer::EpochElse(int status, int row, int column,
    std::string* string, ParsingException* exception) {
  auto push_back_comment_not_end_error = [&]() {
    if (exception != nullptr) {
      *exception = ParsingException {
        "a string must have end double quotation mark "
        "or in a single line",
        source_provider_->LookRow(),
        source_provider_->LookColumn(),
      };
    }
    return false;
  };
  auto push_back_string_not_end_error = [&]() {
    if (exception != nullptr) {
      *exception = ParsingException {
        "a multi-line comment must have its end mark '*/'",
        source_provider_->LookRow(),
        source_provider_->LookColumn(),
      };
    }
    return false;
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
          return true;
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
          return true;
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
          return true;
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
          return true;
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
          return true;
        } else {
          PushBackToken(
              *string,
              Token::Type() <<
                  Token::Type::kOperator <<
                  Token::Type::kOperatorAnd,
              row,
              column);
          return true;
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
          return true;
        } else {
          source_provider_->MoveNext();
          PushBackToken(
              *string,
              Token::Type() <<
                  Token::Type::kOperator <<
                  Token::Type::kOperatorOr,
              row,
              column);
          return true;
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
          return true;
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
          return true;
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
          return false;
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
          return true;
        } else if (c != '\0') {
          *string += c;
          source_provider_->MoveNext();
          status = 11;
        } else {
          push_back_string_not_end_error();
          return false;
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
          return true;
        } else if (c == '\\') {
          source_provider_->MoveNext();
          status = 14;
        } else if (c != '\n' && c != '\0') {
          *string += c;
          source_provider_->MoveNext();
        } else {
          push_back_comment_not_end_error();
          return false;
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
          return false;
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
          return false;
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
  tokens_.push_back(Token{string, type, row, column});
}

}  // namespace vegetable_script
