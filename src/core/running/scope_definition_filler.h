#ifndef VEGETABLE_SCRIPT_CORE_RUNNING_SCOPE_DEFINITION_FILLER_H_
#define VEGETABLE_SCRIPT_CORE_RUNNING_SCOPE_DEFINITION_FILLER_H_

#include "./scope.h"
#include "../ast/all.h"

namespace vegetable_script {

class ScopeDefinitionFiller {
 public:
  ScopeDefinitionFiller(Ast* ast, Scope* scope);
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_RUNNING_SCOPE_DEFINITION_FILLER_H_
