#ifndef RANDOM_BSUFCP9FQZ7LMD2KWSNXS0VQT8HL54KMHCX17HABOK62GLTSJOEWJ5EA0CP
#define RANDOM_BSUFCP9FQZ7LMD2KWSNXS0VQT8HL54KMHCX17HABOK62GLTSJOEWJ5EA0CP

#include "../../ast/all.h"
#include "./double_exception.h"
#include "../environment/scope_stack.h"
#include "../environment/statement_result.h"

namespace vegetable_script {

class DoubleStatementInvoker : public IVisitor {
 public:
  explicit DoubleStatementInvoker(ScopeStack* scope_stack);

  void Visit(ExpressionStatement* node) override;
  void Visit(BlockStatement* node) override;
  void Visit(IfStatement* node) override;
  void Visit(WhileStatement* node) override;
  void Visit(ForStatement* node) override;
  void Visit(ContinueStatement* node) override;
  void Visit(BreakStatement* node) override;
  void Visit(ReturnStatement* node) override;

  bool success() { return success_; }
  DoubleException exception() { return exception_; }
  StatementResult result() { return result_; }

 private:
  bool success_ = true;
  DoubleException exception_;
  StatementResult result_;
  ScopeStack* scope_stack_;
};

}  // namespace vegetable_script

#endif  // RANDOM_BSUFCP9FQZ7LMD2KWSNXS0VQT8HL54KMHCX17HABOK62GLTSJOEWJ5EA0CP
