#ifndef RANDOM_TMGZKFFHKPRRXHRVDIFYE5TLZIQXW3DKCVUXU9EXJFNKNNJPTM1SRZXEGKSTIBG
#define RANDOM_TMGZKFFHKPRRXHRVDIFYE5TLZIQXW3DKCVUXU9EXJFNKNNJPTM1SRZXEGKSTIBG

#include <deque>
#include "./expression_result.h"
#include "./statement_result.h"
#include "../../ast/all.h"
#include <memory>
#include <string>

namespace vegetable_script {

class ScopeStack;

class FunctionSymbol {
 public:
  using Ptr = std::shared_ptr<FunctionSymbol>;
  using Argument = ExpressionResult;
  using ArgumentList = std::deque<Argument>;
  using ReturnResult = StatementResult;

  virtual bool Invoke(ArgumentList* argument_list, ScopeStack* scope_stack, ReturnResult* result) = 0;
  virtual std::string GetName() = 0;
};

class NativeFunctionSymbol : public FunctionSymbol {
 public:
  virtual bool Invoke(ArgumentList* argument_list, ScopeStack* scope_stack, ReturnResult* result) = 0;
  virtual std::string GetName() = 0;

  static void FillScopeStack(ScopeStack* scope_stack);
};

class CustomFunctionSymbol : public FunctionSymbol {
 public:
  explicit CustomFunctionSymbol(FunctionDefinition::Ptr definition);

  bool Invoke(ArgumentList* argument_list, ScopeStack* scope_stack, ReturnResult* result) override;
  std::string GetName() override;

  static void FillScopeStack(Ast* ast, ScopeStack* scope_stack);

 private:
  FunctionDefinition::Ptr definition_;
};

}  // namespace vegetable_script

#endif  // RANDOM_TMGZKFFHKPRRXHRVDIFYE5TLZIQXW3DKCVUXU9EXJFNKNNJPTM1SRZXEGKSTIBG
