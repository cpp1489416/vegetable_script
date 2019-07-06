#include "./scope_stack.h"

namespace vegetable_script {

ScopeStack::ScopeStack() : scope_(std::make_shared<Scope>()) { }

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

