#ifndef RANDOM_BSUFCP9FQZ7LMD2KWSNXS0VQT8HL54KMHCX17HABOK62GLTSJOEWJ5EA0CP
#define RANDOM_BSUFCP9FQZ7LMD2KWSNXS0VQT8HL54KMHCX17HABOK62GLTSJOEWJ5EA0CP

#include "../../ast/all.h"
#include "./double_exception.h"
#include "../scope_stack.h"

namespace vegetable_script {

class DoubleStatementInvoker : public IVisitor {
 public:
  explicit DoubleStatementInvoker(ScopeStack* scope_stack);

  virtual void Visit(ExpressionStatement* node);

  virtual void Visit(BlockStatement* node);

  bool success() { return success_; }

  DoubleException exception() { return exception_; }

  double result() { return result_; }

 private:
  bool success_ = true;

  DoubleException exception_;

  double result_ = 0.0;

  ScopeStack* scope_stack_;

  std::string assignable_;
};

}  // namespace vegetable_script

#endif  // RANDOM_BSUFCP9FQZ7LMD2KWSNXS0VQT8HL54KMHCX17HABOK62GLTSJOEWJ5EA0CP
