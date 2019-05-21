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

}  // namespace vegetable_script
