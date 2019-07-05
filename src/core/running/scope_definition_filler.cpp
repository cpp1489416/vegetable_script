#include "./scope_definition_filler.h"
#include <memory>

namespace vegetable_script {

namespace {
class Filler : public IVisitor {
 public:
  explicit Filler(Scope* scope) : scope_(scope) {}

  virtual void Visit(BlockStatement* node) {
    for (auto statement : node->statements) {
      statement->Accept(this);
    }
  }

  virtual void Visit(FunctionDefinition* node) {
    scope_->PutDefinition(node->name, std::static_pointer_cast<FunctionDefinition>(node->shared_from_this()));
  }

 private:
  Scope* scope_;
};
}  // namespace

ScopeDefinitionFiller::ScopeDefinitionFiller(Ast* ast, Scope* scope) {
  Filler filler(scope);
  ast->Accept(&filler);
}

}  // namespace vegetable_script
