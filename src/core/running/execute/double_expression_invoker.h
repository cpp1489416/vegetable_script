#ifndef RANDOM_EUIBTKBQ2QZ06BCUF0NY2QS0JOH98MQC16LRQNJKEKFR5AUOSAP
#define RANDOM_EUIBTKBQ2QZ06BCUF0NY2QS0JOH98MQC16LRQNJKEKFR5AUOSAP

#include "./double_exception.h"
#include "../environment/statement_result.h"
#include "../../ast/all.h"
#include <string>

namespace vegetable_script {

class ScopeStack;

class DoubleExpressionInvoker : public IVisitor {
 public:
  explicit DoubleExpressionInvoker(ScopeStack* scope_stack);

  void Visit(IdentifierExpression* node) override;
  void Visit(StringExpression* node) override;
  void Visit(IntegerExpression* node) override;
  void Visit(FloatExpression* node) override;
  void Visit(UnaryExpression* node) override;
  void Visit(BinaryExpression* node) override;
  void Visit(FunctionInvokeExpression* node) override;

  void EvalBinaryBool(BinaryExpression* node, StatementResult* left_result, StatementResult* right_result);
  void EvalBinaryInt(BinaryExpression* node, StatementResult* left_result, StatementResult* right_result);
  void EvalBinaryFloat(BinaryExpression* node, StatementResult* left_result, StatementResult* right_result);
  void EvalBinaryString(BinaryExpression* node, StatementResult* left_result, StatementResult* right_result);

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

#endif  // RANDOM_EUIBTKBQ2QZ06BCUF0NY2QS0JOH98MQC16LRQNJKEKFR5AUOSAP
