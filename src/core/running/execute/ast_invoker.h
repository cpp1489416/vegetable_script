#ifndef AFJDIOAFJODASJFIODSAJIFOPADSFJA_H_
#define AFJDIOAFJODASJFIODSAJIFOPADSFJA_H_

#include "../scope.h"
#include "../scope_stack.h"
#include "../../ast/all.h"

namespace vegetable_script {

class AstInvoker: public IVisitor {
 public:
  explicit AstInvoker(Scope::Ptr scope = nullptr);



 private:
  ScopeStack scope_stack_;
};

}  // namespace vegetable_script

#endif  // AFJDIOAFJODASJFIODSAJIFOPADSFJA_H_

