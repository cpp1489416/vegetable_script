#ifndef VEGETABLE_SCRIPT_CORE_AST_CORE_ASTS_H_
#define VEGETABLE_SCRIPT_CORE_AST_CORE_ASTS_H_

#include <string>
#include <vector>
#include <memory>
#include "../parsing/token.h"
#include "./ast_declarations.h"
#include "./expression_asts.h"

namespace vegetable_script {


struct Statement : public Ast {
};

struct ExpressionStatement : public Statement {
  void Accept(IVisitor* visitor) override {
    visitor->Visit(this);
  }

  Pointer<Expression> expression;
};

struct BlockStatement : public Statement {
  void Accept(IVisitor* visitor) override {
    visitor->Visit(this);
  }

  std::vector<Pointer<Statement> > statements;
};

struct IfStatement : public Statement {
  void Accept(IVisitor* visitor) override {
    visitor->Visit(this);
  }

  Pointer<Expression> condition_expression;
  Pointer<Statement> main_statemenet;
  Pointer<Statement> else_statement;
};

struct WhileStatement : public Statement {
  void Accept(IVisitor* visitor) override {
    visitor->Visit(this);
  }

  Pointer<Expression> condition_expression;
  Pointer<Statement> body_statement;
};

struct ForStatement : public Statement {
  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Pointer<Expression> begin_statement;
  Pointer<Expression> condition_expression;
  Pointer<Expression> after_expression;
  Pointer<Statement> body_statement;
};

struct VariableDefinition : public Ast {
  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  std::string name;
  Pointer<Expression> value_expression;
  Pointer<TypeReference> type_reference;
  Pointer<TypeAst> type;
};

struct ParameterDefinition : public Ast {
  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

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
