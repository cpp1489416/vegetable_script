#include "./statement_result.h"

namespace vegetable_script {

StatementResult::StatementResult()
    : value_type(ValueType::kVoid) {}

StatementResult::StatementResult(bool value)
    : value_type(ValueType::kBool) {
  this->value.bool_value = value;
}

StatementResult::StatementResult(int value)
    : value_type(ValueType::kInt) {
  this->value.int_value = value;
}

StatementResult::StatementResult(double value)
    : value_type(ValueType::kFloat) {
  this->value.float_value = value;
}

StatementResult::StatementResult(const std::string& value)
    :value_type(ValueType::kString) {
  new(static_cast<void*>(&this->value.string_value)) std::string(value);
}

StatementResult::StatementResult(const std::string& value, bool identifier_mark)
    :value_type(ValueType::kIdentifier) {
  new(static_cast<void*>(&this->value.string_value)) std::string(value);
}

StatementResult::StatementResult(const StatementResult& rhs)
    : value_type(rhs.value_type) {
  switch (rhs.value_type) {
    case ValueType::kVoid: {
      return;
    }
    case ValueType::kBool: {
      value.bool_value = rhs.value.bool_value;
      return;
    }
    case ValueType::kInt: {
      value.int_value = rhs.value.int_value;
      return;
    }
    case ValueType::kFloat: {
      value.float_value = rhs.value.float_value;
      return;
    }
    case ValueType::kString: {
      new(static_cast<void*>(&value.string_value)) std::string(rhs.value.string_value);
      return;
    }
    case ValueType::kIdentifier: {
      new(static_cast<void*>(&value.string_value)) std::string(rhs.value.string_value);
      return;
    }
    default: {
      return;
    }
  }
}

StatementResult& StatementResult::operator=(const StatementResult& rhs) {
  if (value_type == ValueType::kString || value_type == ValueType::kIdentifier) {
    value.string_value.~basic_string();
  }
  value_type = rhs.value_type;
  switch (rhs.value_type) {
    case ValueType::kVoid: {
      return *this;
    }
    case ValueType::kBool: {
      value.bool_value = rhs.value.bool_value;
      return *this;
    }
    case ValueType::kInt: {
      value.int_value = rhs.value.int_value;
      return *this;
    }
    case ValueType::kFloat: {
      value.float_value = rhs.value.float_value;
      return *this;
    }
    case ValueType::kString: {
      new(static_cast<void*>(&value.string_value)) std::string(rhs.value.string_value);
      return *this;
    }
    case ValueType::kIdentifier: {
      new(static_cast<void*>(&value.string_value)) std::string(rhs.value.string_value);
      return *this;
    }
    default: {
      return *this;
    }
  }
}

StatementResult::~StatementResult() {
  if (value_type == ValueType::kString || value_type == ValueType::kIdentifier) {
    value.string_value.~basic_string();
  }
}

bool StatementResult::ToBool(bool* result, Exception* exception) {
  switch (value_type) {
    case ValueType::kVoid: {
      *exception = {
        "void value cannot be castted into bool",
      };
      return false;
    }
    case ValueType::kBool: {
      *result = value.bool_value;
      return true;
    }
    case ValueType::kInt: {
      *result = value.int_value == 0;
      return true;
    }
    default: {
      *result = false;
      return true;
    }
  }
}

bool StatementResult::ToInt(int* result, Exception* exception) {
  switch (value_type) {
    case ValueType::kVoid: {
      *exception = { "void type cannot be castted into int" };
      return false;
    }
    case ValueType::kBool: {
      *result = value.bool_value == true ? 1 : 0;
      return true;
    }
    case ValueType::kInt: {
      *result = value.int_value;
      return true;
    }
    case ValueType::kFloat: {
      *result = static_cast<int>(value.float_value);
      return true;
    }
    case ValueType::kString: {
      *exception = { "string cannot be castted into int" };
      return false;
    }
  }
}

bool StatementResult::ToFloat(double* result, Exception* exception) {
  switch (value_type) {
    case ValueType::kVoid: {
      *exception = { "void type cannot be castted into float" };
      return false;
    }
    case ValueType::kBool: {
      *result = value.bool_value == true ? 1.0 : 0.0;
      return true;
    }
    case ValueType::kInt: {
      *result = value.int_value;
      return true;
    }
    case ValueType::kFloat: {
      *result = value.float_value;
      return true;
    }
    case ValueType::kString: {
      *exception = { "string cannot be castted into float" };
      return false;
    }
  }
}

bool StatementResult::ToString(std::string* string, Exception* exception) {
  std::stringstream ss;
  switch (value_type) {
    case ValueType::kVoid: {
      ss << "/* void */";
      break;
    }
    case ValueType::kBool: {
      ss << value.bool_value;
      break;
    }
    case ValueType::kInt: {
      ss << value.int_value;
      break;
    }
    case ValueType::kFloat: {
      ss << value.float_value;
      break;
    }
    case ValueType::kString: {
      ss << value.string_value;
      break;
    }
  }
  *string = ss.str();
  return true;
}
}  // namespace vegetable_script
