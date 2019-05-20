#include "token.h"

namespace vegetable_script {

Token::Token(std::string string, Token::Type type) {
  string_ = string;
  type_ = type;
}

std::string Token::ToString() const {
  return "Token(" + string_ + ", " + TypeToString(type_) + ")";
}

std::string Token::TypeToString(Token::Type type) {
  return type.ToString();
}

}  // namespace vegetable_script
