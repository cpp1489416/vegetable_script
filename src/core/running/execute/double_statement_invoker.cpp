#include "./double_statement_invoker.h"
#include "./double_expression_invoker.h"
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
    result_ = expression_invoker.result_;
    std::cout << "result: " << result_ << std::endl;
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

}  // namespace vegetable_script
