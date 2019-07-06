#include "./double_expression_invoker.h"
#include "./double_statement_invoker.h"
#include "../environment/scope_stack.h"
#include "../environment/scope.h"
#include <iostream>

namespace vegetable_script {

DoubleExpressionInvoker::DoubleExpressionInvoker(ScopeStack* scope_stack)
    : scope_stack_(scope_stack), success_(true) {
}

void DoubleExpressionInvoker::Visit(IdentifierExpression* node) {
  result_ =  StatementResult(node->value, bool());
}

void DoubleExpressionInvoker::Visit(StringExpression* node) {
  result_ = StatementResult(node->value);
}

void DoubleExpressionInvoker::Visit(IntegerExpression* node) {
  result_ = StatementResult(node->value);
}

void DoubleExpressionInvoker::Visit(FloatExpression* node) {
  result_ = StatementResult(node->value);
}

void DoubleExpressionInvoker::Visit(UnaryExpression* node) {
  node->operand_expression->Accept(this);
  switch (result_.value_type) {
    case StatementResult::ValueType::kVoid: {
      success_ = false;
      break;
    }
    case StatementResult::ValueType::kInt: {
      if (node->operatorr == UnaryExpression::Operator::kNegative) {
        result_.value.int_value *= -1;
      }
      break;
    }
    case StatementResult::ValueType::kFloat: {
      if (node->operatorr == UnaryExpression::Operator::kNegative) {
        result_.value.float_value *= -1.0;
      }
      break;
    }
    case StatementResult::ValueType::kString: {
      success_ = false;
      break;
    }
    case StatementResult::ValueType::kIdentifier: {
      result_ = scope_stack_->scope()->GetVariable(result_.value.string_value);
      Visit(node);
      break;
    }
  }
}

void DoubleExpressionInvoker::Visit(BinaryExpression* node) {
  node->left_expression->Accept(this);
  StatementResult left_result = result_;
  node->right_expression->Accept(this);
  StatementResult right_result = result_;

  // make sure right value is not left value
  while (right_result.value_type == StatementResult::ValueType::kIdentifier) {
    right_result = scope_stack_->scope()->GetVariable(right_result.value.string_value);
  }

  if (node->operatorr == BinaryExpression::Operator::kAssign) {
    if (left_result.value_type != StatementResult::ValueType::kIdentifier) {
      success_ = false;
      exception_ = { "left value is incorrect" };
      return;
    } else {
      scope_stack_->scope()->PutVariable(left_result.value.string_value, right_result);
    }
  } else {
    while (left_result.value_type == StatementResult::ValueType::kIdentifier) {
      left_result = scope_stack_->scope()->GetVariable(left_result.value.string_value);
    }

    if (left_result.value_type == StatementResult::ValueType::kBool) {
      if (right_result.value_type != StatementResult::ValueType::kBool) {
        success_ = false;
        exception_ = { "cannot left bool and right not bool" };
        return;
      }
      EvalBinaryBool(node, &left_result, &right_result);
    } else if (left_result.value_type == StatementResult::ValueType::kInt &&
        right_result.value_type == StatementResult::ValueType::kInt) {
      EvalBinaryInt(node, &left_result, &right_result);
    } else if (left_result.value_type == StatementResult::ValueType::kInt &&
        right_result.value_type == StatementResult::ValueType::kFloat) {
      left_result = StatementResult(static_cast<double>(left_result.value.int_value));
      EvalBinaryFloat(node, &left_result, &right_result);
    } else if (left_result.value_type == StatementResult::ValueType::kFloat &&
        right_result.value_type == StatementResult::ValueType::kInt) {
      right_result = StatementResult(static_cast<double>(right_result.value.int_value));
      EvalBinaryFloat(node, &left_result, &right_result);
    }
  }
}

void DoubleExpressionInvoker::Visit(FunctionInvokeExpression* node) {
  FunctionSymbol::Ptr function_symbol = scope_stack_->scope()->GetFunctionSymbol(node->function_name);
  FunctionSymbol::ArgumentList argument_list;
  if (!function_symbol) {
    success_ = false;
    exception_ = { std::string() + "function " + node->function_name + " is not found" };
    return;
  }
  for (size_t i = 0; i < node->argument_list.size(); ++i) {
    node->argument_list[i]->Accept(this);
    argument_list.push_back(result_);
  }
  success_ = function_symbol->Invoke(&argument_list, scope_stack_, &result_);
}

void DoubleExpressionInvoker::EvalBinaryBool(
    BinaryExpression* node,
    StatementResult* left_result,
    StatementResult* right_result) {
}

void DoubleExpressionInvoker::EvalBinaryInt(
    BinaryExpression* node,
    StatementResult* left_result,
    StatementResult* right_result) {
  if (node->operatorr == BinaryExpression::Operator::kPlus) {
    result_ = StatementResult(left_result->value.int_value + right_result->value.int_value);
  } else if (node->operatorr == BinaryExpression::Operator::kMinus) {
    result_ = StatementResult(left_result->value.int_value - right_result->value.int_value);
  } else if (node->operatorr == BinaryExpression::Operator::kMultiply) {
    result_ = StatementResult(left_result->value.int_value * right_result->value.int_value);
  } else if (node->operatorr == BinaryExpression::Operator::kDivide) {
    if (right_result->value.int_value == 0) {
      success_ = false;
      exception_ = { "cannot divide by zero" };
      return;
    }
    result_ = StatementResult(left_result->value.int_value / right_result->value.int_value);
  }
}

void DoubleExpressionInvoker::EvalBinaryFloat(
    BinaryExpression* node,
    StatementResult* left_result,
    StatementResult* right_result) {
  if (node->operatorr == BinaryExpression::Operator::kPlus) {
    result_ = StatementResult(left_result->value.float_value + right_result->value.float_value);
  } else if (node->operatorr == BinaryExpression::Operator::kMinus) {
    result_ = StatementResult(left_result->value.float_value - right_result->value.float_value);
  } else if (node->operatorr == BinaryExpression::Operator::kMultiply) {
    result_ = StatementResult(left_result->value.float_value * right_result->value.float_value);
  } else if (node->operatorr == BinaryExpression::Operator::kDivide) {
    result_ = StatementResult(left_result->value.float_value / right_result->value.float_value);
  }
}

void DoubleExpressionInvoker::EvalBinaryString(
    BinaryExpression* node,
    StatementResult* left_result,
    StatementResult* right_result) {
}

}  // namespace vegetable_script
