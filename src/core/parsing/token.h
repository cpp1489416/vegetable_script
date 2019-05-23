#ifndef VEGETABLE_SCRIPT_CORE_PARSING_TOKEN_H_
#define VEGETABLE_SCRIPT_CORE_PARSING_TOKEN_H_

#include "../common/all.h"
#include <string>
#include <memory>
#include <initializer_list>
#include <deque>

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
  kOperatorPlusOrPositive, kOperatorMinusOrNegative,
  kOperatorMultiply, kOperatorDivide,
  kOperatorAnd, kOperatorOr,
  kOperatorGreater, kOperatorLesser,
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

  using PtrDeque = std::deque<Ptr>;

  explicit Token(std::string string = "", Type type = Type::kNone,
    int row = -1, int column = -1);

  std::string ToString() const;

  std::string ToEscapedString() const;

  bool MatchType(std::initializer_list<Type::Type_> types);

  bool MatchString(const std::string& string);

  std::string string;

  Type type;

  int row;

  int column;
};

}  // namespace vegetable_script

#endif
