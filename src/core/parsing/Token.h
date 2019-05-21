#ifndef VEGETABLE_SCRIPT_CORE_PARSING_TOKEN_H_
#define VEGETABLE_SCRIPT_CORE_PARSING_TOKEN_H_

#include "../common/all.h"
#include <string>
#include <memory>

namespace vegetable_script {

namespace detail {

XC_MULTI_ENUM(
  TokenType,

  kNone,
  kEnd,
  kError,
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

}  // namespace detail

class Token {
 public:
  using Type = detail::TokenType;

  using Pointer = std::shared_ptr<Token>;

  using Ptr = Pointer;

  explicit Token(std::string string = "", Type type = Type::kNone,
    int row = -1, int column = -1);

  std::string ToString() const;

  std::string ToEscapedString() const;

  std::string string;

  Type type;

  int row;

  int column;
};

}  // namespace vegetable_script

#endif
