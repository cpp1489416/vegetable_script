#ifndef VEGETABLE_SCRIPT_CORE_AST_STATEMENT_ASTS_H_
#define VEGETABLE_SCRIPT_CORE_AST_STATEMENT_ASTS_H_

#include <string>
#include <vector>
#include <memory>
#include "../parsing/token.h"
#include "./ast_declarations.h"
#include "./expression_asts.h"

namespace vegetable_script  {

struct Statement : public Ast {
  using Ptr = std::shared_ptr<Statement>;
  using List = std::vector<Ptr>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }
};

struct EmptyStatement : public Statement {
  using Ptr = std::shared_ptr<EmptyStatement>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }
};

struct ExpressionStatement : public Statement {
  using Ptr = std::shared_ptr<ExpressionStatement>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Expression::Ptr expression;
};

struct BlockStatement : public Statement {
  using Ptr = std::shared_ptr<BlockStatement>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Statement::List statements;
};

struct IfStatement : public Statement {
  using Ptr = std::shared_ptr<IfStatement>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Expression::Ptr condition_expression;
  Statement::Ptr body_statement;
  Statement::Ptr else_statement;
};

struct WhileStatement : public Statement {
  using Ptr = std::shared_ptr<WhileStatement>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Expression::Ptr condition_expression;
  Statement::Ptr body_statement;
};

struct ForStatement : public Statement {
  using Ptr = std::shared_ptr<ForStatement>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Statement::Ptr begin_statement;
  Expression::Ptr condition_expression;
  Expression::Ptr after_expression;
  Statement::Ptr body_statement;
};

struct ContinueStatement : public Statement {
  using Ptr = std::shared_ptr<ContinueStatement>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }
};

struct BreakStatement : public Statement {
  using Ptr = std::shared_ptr<BreakStatement>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }
};

struct ReturnStatement : public Statement {
  using Ptr = std::shared_ptr<ReturnStatement>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_AST_STATEMENT_ASTS_H_
