#include "./scope.h"

namespace vegetable_script {

Scope::Scope(Scope::Ptr parent_scope): parent_scope_(parent_scope) {
}

void Scope::PutDefinition(std::string name, Definition::Ptr definition) {
  definitions_[name] = definition;
}

void Scope::PutVariable(std::string name, double value) {
  variables_[name] = value;
}

Definition::Ptr Scope::GetDefinition(std::string name) {
  if (definitions_.count(name) != 0) {
    return definitions_[name];
  } else if (parent_scope_) {
    return parent_scope_->GetDefinition(name);
  } else {
    return nullptr;
  }
}

double Scope::GetVariable(std::string name) {
  if (variables_.count(name) != 0) {
    return variables_[name];
  } else if (parent_scope_) {
    return parent_scope_->GetVariable(name);
  } else {
    return 0;
  }
}

bool Scope::VariableExists(std::string name) {
  if (variables_.count(name) != 0) {
    return true;
  } else if (parent_scope_) {
    return parent_scope_->VariableExists(name);
  } else {
    return false;
  }
}

}  // namespace vegetable_script
