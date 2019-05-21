
#include "./lexer.h"
#include <cstring>

namespace {
bool IsBracket(char c) {
  return c == '(' || c == ')' || c == '{' || c == '}';
}

bool IsBlank(char c) {
  return c == ' ' || c == '\n' || c == '\r';
}

bool IsDigit(char c) {
  return isdigit(c);
}

bool IsAlpha(char c) {
  return isalpha(c);
}

bool IsNumber(char c) {
  return c >= '0' && c <= '9';
}

bool IsSymbol(char c) {
  return !IsDigit(c) && !IsAlpha(c) && !IsNumber(c) &&
    !IsBracket(c) && !IsBlank(c) && c != '\0';
}

}  // namespace

namespace vegetable_script {

bool Lexer::IsHasNext() const {
  return current_index_ < token_array_.size();
}

void Lexer::MoveToFront() {
  current_index_ = 0;
}

Token Lexer::GetNextToken() {
  if (current_index_ >= token_array_.size()) {
    throw "error";
  }

  return token_array_[current_index_++];
}

Token Lexer::GetPreviousToken() {
  return  token_array_[--current_index_];
}

Token Lexer::LookAhead(int more) {
  if (current_index_ + more - 1>= token_array_.size()) {
    return Token("END", Token::Type::kEnd);
  }

  return token_array_[current_index_ + more - 1];
}

void Lexer::Solve() {
  int position = 0;
  while (true) {
    if (position >= source_code_.size()) {
      break;
    }

    char c = source_code_[position];
    std::string newString;

    if (IsBlank(c)) {
      ++position;
      continue;
    }

    if (c == '\0') {
      break;
    }

    if (c == '-') {
      if (IsNumber(source_code_[position + 1])) {
        newString = "-";
        ++position;

        while (IsNumber(source_code_[position])) {
          newString += source_code_[position];
          ++position;
        }

        AddString(newString);
        continue;
      } else {
        AddString("-");
        ++position;
        continue;
      }
    } else if (IsDigit(c)) {
      std::string newString;
      while (IsDigit(source_code_[position])) {
        newString +=source_code_[position];
        ++position;
      }

      AddString(newString);
      continue;
    } else if (IsBracket(c)) {
      AddString(std::string(1, c));
      ++position;
      continue;
    } else if (IsAlpha(c)) {
      std::string newString;
      while (IsNumber(source_code_[position]) ||
        IsAlpha(source_code_[position])) {
        newString += source_code_[position];
        ++position;
      }

      AddString(newString);
      continue;
    } else if (c == ';') {
      ++position;
      AddString(";");
      continue;
    } else if (IsSymbol(c)) {
      std::string newString;
      while (IsSymbol(source_code_[position])) {
        newString += source_code_[position];
        ++position;
      }
      AddString(newString);
      continue;
    } else {
      throw "Error";
    }
  }

  token_array_.push_back(Token("", Token::Type::kEnd));
}

void Lexer::AddString(std::string s) {
  token_array_.push_back(ConvertStringToToken(s));
}

Token Lexer::ConvertStringToToken(std::string s) {
  if (s == "(" || s == ")") {
    return Token(s,
      Token::Type() << Token::Type::kBracket << Token::Type::kBracketLeft);
  } else if (IsAlpha(s[0])) {
    return Token(s, Token::Type::kName);
  } else if (IsDigit(s[0]) || s[0] == '-' && s.length() > 1 &&
    IsDigit(s[1])) {
    return Token(s, Token::Type::kNumber);
  } else {
    return Token(s, Token::Type::kNone);
  }
}

}  // namespace vegetable_script
