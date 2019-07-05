#ifndef VEGETABLE_SCRIPT_CORE_AST_DEFINITION_ASTS_H_
#define VEGETABLE_SCRIPT_CORE_AST_DEFINITION_ASTS_H_

#include <string>
#include <vector>
#include <memory>
#include "../parsing/token.h"
#include "./ast_declarations.h"
#include "./statement_asts.h"

namespace vegetable_script {

struct Definition : public Statement {
  using Ptr = std::shared_ptr<Definition>;
};

struct VariableDefinition : public Definition {
  using Ptr = std::shared_ptr<VariableDefinition>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  std::string name;
  Expression::Ptr value_expression;
  Pointer<TypeReference> type_reference;
  Pointer<TypeAst> type;
};

/*
struct ParameterDefinition : public Ast {
  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Pointer<VariableExpression> variable_expression;
  bool is_my_reference = false;
};
*/

struct FunctionDefinition : public Definition {
  using Ptr = std::shared_ptr<FunctionDefinition>;
  using ParameterList = std::vector<VariableDefinition::Ptr>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  std::string name;
  ParameterList parameter_list;
  BlockStatement::Ptr block_statement;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_AST_DEFINITION_ASTS_H_
