#ifndef VEGETABLE_SCRIPT_CORE_RUNNING_SCOPE_H_
#define VEGETABLE_SCRIPT_CORE_RUNNING_SCOPE_H_

#include <memory>
#include <map>
#include <string>
#include "../ast/all.h"

namespace vegetable_script {

class Scope {
 public:
  using Ptr = std::shared_ptr<Scope>;

  explicit Scope(Ptr parent_scope = nullptr);

  void PutDefinition(std::string name, Definition::Ptr definition);

  void PutVariable(std::string name, double value);

  Definition::Ptr GetDefinition(std::string name);

  double GetVariable(std::string name);

  bool VariableExists(std::string name);

  void set_parent_scope(Ptr scope) { parent_scope_ = scope; }

  Ptr parent_scope() { return parent_scope_; }

 private:
  Ptr parent_scope_;
  std::map<std::string, double> variables_;
  std::map<std::string, Definition::Ptr> definitions_;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_RUNNING_SCOPE_H_
