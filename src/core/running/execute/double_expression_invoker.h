#ifndef RANDOM_EUIBTKBQ2QZ06BCUF0NY2QS0JOH98MQC16LRQNJKEKFR5AUOSAP
#define RANDOM_EUIBTKBQ2QZ06BCUF0NY2QS0JOH98MQC16LRQNJKEKFR5AUOSAP

#include "../scope_definition_filler.h"
#include "../scope_stack.h"
#include "./double_exception.h"
#include <string>

namespace vegetable_script {

class DoubleExpressionInvoker : public IVisitor {
 public:
  explicit DoubleExpressionInvoker(ScopeStack* scope_stack);

  virtual void Visit(IdentifierExpression* node);

  virtual void Visit(StringExpression* node);

  virtual void Visit(NumberExpression* node);

  virtual void Visit(IntegerExpression* node);

  virtual void Visit(FloatExpression* node);

  virtual void Visit(UnaryExpression* node);

  virtual void Visit(BinaryExpression* node);

  virtual void Visit(FunctionInvokeExpression* node);

  bool success() { return success_; }

  DoubleException exception() { return exception_; }

  double result() { return result_; }

 public:
  bool success_ = true;

  DoubleException exception_;

  double result_ = 0.0;

  ScopeStack* scope_stack_;

  std::string assignable_;
};

}  // namespace vegetable_script

#endif  // RANDOM_EUIBTKBQ2QZ06BCUF0NY2QS0JOH98MQC16LRQNJKEKFR5AUOSAP
