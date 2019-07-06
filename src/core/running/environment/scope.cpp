#include "./scope.h"

namespace vegetable_script {

Scope::Scope(Scope::Ptr parent_scope): parent_scope_(parent_scope) {
}

void Scope::PutVariable(std::string name, StatementResult value) {
  variables_[name] = value;
}

StatementResult Scope::GetVariable(std::string name) {
  if (variables_.count(name) != 0) {
    return variables_[name];
  } else if (parent_scope_) {
    return parent_scope_->GetVariable(name);
  } else {
    return StatementResult();
  }
}

void Scope::PutFunctionSymbol(FunctionSymbol::Ptr function_symbol) {
  function_symbols_[function_symbol->GetName()] = function_symbol;
}

FunctionSymbol::Ptr Scope::GetFunctionSymbol(const std::string& name) {
  if (function_symbols_.count(name) != 0) {
    return function_symbols_[name];
  } else if (parent_scope_) {
    return parent_scope_->GetFunctionSymbol(name);
  } else {
    return nullptr;
  }
}

}  // namespace vegetable_script
