#ifndef RANDOM_BIWZZT7U82ZFOUNWQFFX79HILCQLMLUD4KN9Y7OTUFH7M0YZSQXFLE0CIH
#define RANDOM_BIWZZT7U82ZFOUNWQFFX79HILCQLMLUD4KN9Y7OTUFH7M0YZSQXFLE0CIH

#include "../../common/all.h"
#include <string>
#include <sstream>

namespace vegetable_script {

namespace detail {

union ExpressionResultValue {
  ExpressionResultValue() {}
  ~ExpressionResultValue() {}

  bool bool_value;
  int int_value;
  double float_value;
  std::string string_value;
};

}  // namespace detail

class ScopeStack;

struct ExpressionResult final {
  using Value = detail::ExpressionResultValue;

  enum class ValueType {
    kVoid,
    kBool,
    kInt,
    kFloat,
    kString,
    kIdentifier,
  };

  struct Exception {
    std::string status;
  };

  struct IdentifierTag {};

  ExpressionResult();
  explicit ExpressionResult(bool value);
  explicit ExpressionResult(int value);
  explicit ExpressionResult(double value);
  explicit ExpressionResult(const std::string& value);
  explicit ExpressionResult(const std::string& value, bool identifier_mark);
  ExpressionResult(const ExpressionResult& rhs);
  ExpressionResult& operator=(const ExpressionResult& rhs);
  ~ExpressionResult();

  bool ToBool(bool* result, Exception* exception);
  bool ToInt(int* result, Exception* exception);
  bool ToFloat(double* result, Exception* exception);
  bool ToString(std::string* string, Exception* exception);
  bool ToRightValue(ScopeStack* scope_stack, ExpressionResult* result);

  ValueType value_type;  // read only
  Value value;
};

}  // namespace vegetable_script

#endif  // RANDOM_BIWZZT7U82ZFOUNWQFFX79HILCQLMLUD4KN9Y7OTUFH7M0YZSQXFLE0CIH
