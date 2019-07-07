#include "./double_statement_invoker.h"
#include "./double_expression_invoker.h"
#include "../environment/scope_stack.h"
#include "../environment/scope.h"
#include <iostream>

namespace vegetable_script {

DoubleStatementInvoker::DoubleStatementInvoker(ScopeStack* scope_stack)
    : scope_stack_(scope_stack), success_(true) { }

void DoubleStatementInvoker::Visit(ExpressionStatement* node) {
  DoubleExpressionInvoker expression_invoker(scope_stack_);
  node->expression->Accept(&expression_invoker);
  if (!expression_invoker.success()) {
    success_ = false;
    exception_ = expression_invoker.exception();
  } else {
    result_ = StatementResult(expression_invoker.result());
  }
}

void DoubleStatementInvoker::Visit(BlockStatement* node) {
  scope_stack_->PushScope();
  for (size_t i = 0; i < node->statements.size(); ++i) {
    node->statements[i]->Accept(this);
    if (!success_) {
      success_ = false;
      return;
    }
  }
  scope_stack_->PopScope();
}

void DoubleStatementInvoker::Visit(IfStatement* node) {
  DoubleExpressionInvoker expression_invoker(scope_stack_);
  node->condition_expression->Accept(&expression_invoker);
  if (expression_invoker.result().value_type != ExpressionResult::ValueType::kBool) {
    success_ = false;
    return;
  }
  if (expression_invoker.result().value.bool_value) {
    scope_stack_->PushScope();
    node->body_statement->Accept(this);
    scope_stack_->PopScope();
  } else if (node->else_statement) {
    scope_stack_->PushScope();
    node->else_statement->Accept(this);
    scope_stack_->PopScope();
  }
}

void DoubleStatementInvoker::Visit(WhileStatement* node) {
  while (true) {
    DoubleExpressionInvoker expression_invoker(scope_stack_);
    node->condition_expression->Accept(&expression_invoker);
    if (expression_invoker.result().value_type != ExpressionResult::ValueType::kBool) {
      success_ = false;
      break;
    }

    scope_stack_->PushScope();
    if (expression_invoker.result().value.bool_value) {
      node->body_statement->Accept(this);
    } else {
      break;
    }
    scope_stack_->PopScope();
  }
}

void DoubleStatementInvoker::Visit(ForStatement* node) {
  scope_stack_->PushScope();
  node->begin_statement->Accept(this);
  while (true) {
    DoubleExpressionInvoker expression_invoker(scope_stack_);
    node->condition_expression->Accept(&expression_invoker);
    if (!expression_invoker.result().value.bool_value) {
      break;
    }
    scope_stack_->PushScope();
    node->body_statement->Accept(this);
    scope_stack_->PopScope();
    node->after_expression->Accept(&expression_invoker);
  }

  scope_stack_->PopScope();
}

void DoubleStatementInvoker::Visit(ContinueStatement* node) {}

void DoubleStatementInvoker::Visit(BreakStatement* node) {}

void DoubleStatementInvoker::Visit(ReturnStatement* node) {}

}  // namespace vegetable_script
