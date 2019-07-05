#include "./scope_stack.h"

namespace vegetable_script {

ScopeStack::ScopeStack() : scope_(nullptr) {}

void ScopeStack::PutDefinition(std::string name, Definition::Ptr definition) {
  if (scope_) {
    scope_->PutDefinition(name, definition);
  }
}

void ScopeStack::PutVariable(std::string name, double value) {
  if (scope_) {
    scope_->PutVariable(name, value);
  }
}

Definition::Ptr ScopeStack::GetDefinition(std::string name) {
  if (scope_) {
    return scope_->GetDefinition(name);
  } else {
    return nullptr;
  }
}

double ScopeStack::GetVariable(std::string name) {
  if (scope_) {
    return scope_->GetVariable(name);
  } else {
    return 0.0;
  }
}

bool ScopeStack::VariableExists(std::string name) {
  if (scope_) {
    return scope_->VariableExists(name);
  } else {
    return false;
  }
}

void ScopeStack::PushScope() {
  auto scope = std::make_shared<Scope>(scope_);
  scope_ = scope;
}

void ScopeStack::PushScope(Scope::Ptr scope) {
  scope->set_parent_scope(scope_);
  scope_ = scope;
}

bool ScopeStack::PopScope() {
  if (!scope_) {
    return false;
  }
  scope_ = scope_->parent_scope();
  return true;
}

}  // namespace vegetable_script

