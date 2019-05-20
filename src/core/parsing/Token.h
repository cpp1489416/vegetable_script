#ifndef VEGETABLE_SCRIPT_CORE_PARSING_TOKEN_H_
#define VEGETABLE_SCRIPT_CORE_PARSING_TOKEN_H_

#include <string>
#include "../common/all.h"

namespace vegetable_script {

class Token {
 public:
  enum class sTypess {
    kNone, kEnd, kBracket, kNumber, kOperator, kKeyword, kName,  // main type
    kLeftBracket, kRightBracket,
  };
  XC_MULTI_ENUM(
    Type,
    kNone,
    kEnd,

    kBracket,
    kBracketLeft, kBracketRight,
    kBracketBig, kBracketMiddle, kBracketSmall,

    kNumber, kNumberFloat, kNumberInteger,

    kOperator,
    kOperatorPlus, kOperatorMinus,
    kOperatorMultiply, kOperatorDivide,
    kOperatorNegative,

    kKeyword,
    kName,
  )

  explicit Token(std::string string = "", Type type = Type::kNone);

  std::string string() const { return string_; }

  Type type() const { return type_; }

  std::string ToString() const;

 private:
  static std::string TypeToString(Type type);

  std::string string_;

  Type type_;

  friend class Parser;

  friend class Lexer;
};

}  // namespace vegetable_script

#endif
