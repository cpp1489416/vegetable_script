#include "token.h"
#include "../common/all.h"

namespace vegetable_script {

Token::Token(std::string string, Token::Type type, int row, int column):
  string(string), type(type), row(row), column(column) {
}

std::string Token::ToString() const {
  return "Token(\"" + string + "\", " + type.ToString() + ")";
}

std::string Token::ToEscapedString() const {
  return "Token(\"" + xc::StringUtil::Escape(string) + "\", "
      + type.ToString() + ")";
}

bool Token::MatchType(std::initializer_list<Type::Type_> types) {
  for (auto t : types) {
    if (type != t) {
      return false;
    }
  }
  return true;
}

bool Token::MatchString(const std::string& string) {
  return this->string == string;
}

}  // namespace vegetable_script
