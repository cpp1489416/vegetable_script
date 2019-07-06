#ifndef RANDOM_BIWZZT7U82ZFOUNWQFFX79HILCQLMLUD4KN9Y7OTUFH7M0YZSQXFLE0CIH
#define RANDOM_BIWZZT7U82ZFOUNWQFFX79HILCQLMLUD4KN9Y7OTUFH7M0YZSQXFLE0CIH

#include "../../common/all.h"
#include <string>
#include <sstream>

namespace vegetable_script {

namespace detail {

union StatementResultValue {
  StatementResultValue() {}
  ~StatementResultValue() {}

  bool bool_value;
  int int_value;
  double float_value;
  std::string string_value;
};

}  // namespace detail

class ScopeStack;

struct StatementResult {
  using Value = detail::StatementResultValue;

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

  StatementResult();
  explicit StatementResult(bool value);
  explicit StatementResult(int value);
  explicit StatementResult(double value);
  explicit StatementResult(const std::string& value);
  explicit StatementResult(const std::string& value, bool identifier_mark);
  StatementResult(const StatementResult& rhs);
  StatementResult& operator=(const StatementResult& rhs);
  ~StatementResult();

  bool ToBool(bool* result, Exception* exception);
  bool ToInt(int* result, Exception* exception);
  bool ToFloat(double* result, Exception* exception);
  bool ToString(std::string* string, Exception* exception);
  bool ToRightValue(ScopeStack* scope_stack, StatementResult* result);

  ValueType value_type;  // read only
  Value value;
};

}  // namespace vegetable_script

#endif  // RANDOM_BIWZZT7U82ZFOUNWQFFX79HILCQLMLUD4KN9Y7OTUFH7M0YZSQXFLE0CIH
