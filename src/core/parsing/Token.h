#ifndef VEGETABLE_SCRIPT_CORE_PARSING_TOKEN_H_
#define VEGETABLE_SCRIPT_CORE_PARSING_TOKEN_H_

#include <string>
#include "../common/all.h"

namespace vegetable_script {

class Token {
 public:
  XC_MULTI_ENUM(
    Type,

    kNone,
    kEnd,
    kSemicolon,

    kBracket,
    kBracketLeft, kBracketRight,
    kBracketBig, kBracketMedium, kBracketSmall,

    kNumber, kNumberFloat, kNumberInteger,

    kOperator,
    kOperatorPlus, kOperatorMinus,
    kOperatorMultiply, kOperatorDivide,
    kOperatorNegative,
    kOperatorAnd, kOperatorOr,
    kOperatorGreater, kOperatorLess,
    kOperatorShiftLeft, kOperatorShitRight,

    kComment,
    kString,

    kKeyword,
    kKeywordIf, kKeywordElse, kKeywordWhile,

    kIdentifier
  )

  explicit Token(std::string string = "", Type type = Type::kNone,
    int row = -1, int column = -1);

  std::string ToString() const;

  std::string string;

  Type type;

  int row;

  int column;
};

}  // namespace vegetable_script

#endif
