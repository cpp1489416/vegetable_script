#include "token.h"

namespace vegetable_script {

Token::Token(std::string string, Token::Type type, int row, int column):
  string(string), type(type), row(row), column(column) {
}

std::string Token::ToString() const {
  return "Token(\"" + string + "\", " + type.ToString() + ")";
}

std::string Token::ToEscapedString() const {
  return "";
}

bool Token::Match(std::initializer_list<Type::Type_> types) {
  for (auto t : types) {
    if (type != t) {
      return false;
    }
  }
  return true;
}

}  // namespace vegetable_script
