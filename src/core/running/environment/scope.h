#ifndef VEGETABLE_SCRIPT_CORE_RUNNING_SCOPE_H_
#define VEGETABLE_SCRIPT_CORE_RUNNING_SCOPE_H_

#include <memory>
#include <map>
#include <string>
#include "../../ast/all.h"
#include "./functions.h"

namespace vegetable_script {

class Scope {
 public:
  using Ptr = std::shared_ptr<Scope>;

  explicit Scope(Ptr parent_scope = nullptr);

  void PutVariable(std::string name, StatementResult value);
  StatementResult GetVariable(std::string name);

  void PutFunctionSymbol(FunctionSymbol::Ptr function_symbol);
  FunctionSymbol::Ptr GetFunctionSymbol(const std::string& name);

  void set_parent_scope(Ptr scope) { parent_scope_ = scope; }
  Ptr parent_scope() { return parent_scope_; }

 private:
  Ptr parent_scope_;
  std::map<std::string, StatementResult> variables_;
  std::map<std::string, FunctionSymbol::Ptr> function_symbols_;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_RUNNING_SCOPE_H_
