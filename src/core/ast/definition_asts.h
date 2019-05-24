#ifndef VEGETABLE_SCRIPT_CORE_AST_CORE_ASTS_H_
#define VEGETABLE_SCRIPT_CORE_AST_CORE_ASTS_H_

#include <string>
#include <vector>
#include <memory>
#include "../parsing/token.h"

namespace vegetable_script {

struct IVisitable;

struct Ast;

// statements
struct BlockStatement;

struct VariableDefinition;
struct ParameterDefinition;
using Argument = ParameterDefinition;
struct FunctionDefinition;
using ParameterList = std::vector<Pointer<ParameterDefinition> >;


struct VariableDefinition : public Ast {
  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  std::string name;
  Pointer<Expression> value_expression;
  Pointer<TypeReference> type_reference;
  Pointer<TypeAst> type;
};

struct ParameterDefinition : public Ast {
  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Pointer<VariableExpression> variable_expression;
  bool is_my_reference = false;
};

struct FunctionDefinition : public Ast {
  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  std::string name;
  ParameterList parameter_list;
  Pointer<BlockStatement> block_statement;
};

struct Program : public Ast {
  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  std::vector<Pointer<Ast> > asts;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_AST_CORE_ASTS_H_
