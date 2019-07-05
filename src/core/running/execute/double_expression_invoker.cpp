#include "./double_expression_invoker.h"
#include "./double_statement_invoker.h"
#include <iostream>

namespace vegetable_script {

DoubleExpressionInvoker::DoubleExpressionInvoker(ScopeStack* scope_stack)
    : scope_stack_(scope_stack), success_(true) {
}

void DoubleExpressionInvoker::Visit(IdentifierExpression* node) {
  assignable_ = node->value;
  result_ = scope_stack_->GetVariable(assignable_);
}

void DoubleExpressionInvoker::Visit(StringExpression* node) {
}

void DoubleExpressionInvoker::Visit(NumberExpression* node) {
}

void DoubleExpressionInvoker::Visit(IntegerExpression* node) {
  result_ = node->value;
}

void DoubleExpressionInvoker::Visit(FloatExpression* node) {
  result_ = node->value;
}

void Visit(EmptyExpression* node) {
}

void DoubleExpressionInvoker::Visit(UnaryExpression* node) {
  node->operand_expression->Accept(this);
  if (node->operatorr == UnaryExpression::Operator::kPositive) {
  } else if (node->operatorr == UnaryExpression::Operator::kNegative) {
    result_ = -result_;
  } else {
    result_ = result_ == 0 ? 1 : 0;
  }
}

void DoubleExpressionInvoker::Visit(BinaryExpression* node) {
  node->left_expression->Accept(this);
  double left_result = result_;
  std::string left_assignable = assignable_;
  node->right_expression->Accept(this);

  if (node->operatorr == BinaryExpression::Operator::kAssign) {
    scope_stack_->PutVariable(left_assignable, result_);
  } else if (node->operatorr == BinaryExpression::Operator::kPlus) {
    result_ += left_result;
  } else if (node->operatorr == BinaryExpression::Operator::kMinus) {
    result_ = left_result - result_;
  } else if (node->operatorr == BinaryExpression::Operator::kMultiply) {
    result_ = left_result * result_;
  } else if (node->operatorr == BinaryExpression::Operator::kDivide) {
    result_ = left_result / result_;
  } else {
    success_ = false;
    exception_ = {
      std::string() + "operator " + node->operatorr.ToString() + " is not supported"
    };
  }
}

void DoubleExpressionInvoker::Visit(FunctionInvokeExpression* node) {
  FunctionDefinition::Ptr function_definition =
      std::static_pointer_cast<FunctionDefinition>(scope_stack_->GetDefinition(node->function_name));
  if (!function_definition) {
    if (node->function_name == "print") {
      node->argument_list[0]->Accept(this);
      std::cout << "from program: " << result_ << std::endl;
      return;
    } else {
      success_ = false;
      exception_ = {
        std::string() + "function " + node->function_name + " is not found"
      };
      return;
    }
  }
  scope_stack_->PushScope();
  for (size_t i = 0; i < function_definition->parameter_list.size(); ++i) {
    std::string parameter_name = function_definition->parameter_list[i]->name;
    if (node->argument_list.size() < i) {
      success_ = false;
      exception_ = {
        std::string() + "argument length is not enough"
      };
      return;
    }
    node->argument_list[i]->Accept(this);
    scope_stack_->PutVariable(parameter_name, result_);
  }
  DoubleStatementInvoker double_statement_invoker(scope_stack_);
  function_definition->block_statement->Accept(&double_statement_invoker);
  if (!double_statement_invoker.success()) {
    success_ = false;
    exception_ = double_statement_invoker.exception();
  } else {
    result_ = double_statement_invoker.result();
  }
  scope_stack_->PopScope();
}

}  // namespace vegetable_script
