#ifndef VEGETABLE_SCRIPT_CORE_AST_CORE_ASTS_H_
#define VEGETABLE_SCRIPT_CORE_AST_CORE_ASTS_H_

#include <string>
#include <vector>
#include "../util/pointer.h"
#include "../parsing/token.h"

namespace vegetable_script {

struct IVisitable;

struct Ast;

// expressions
struct Expression;
struct EmptyExpression;
struct LHSExpression;
struct VariableExpression;
struct LiteralExpression;
struct NumberExpression;
struct FunctionExpression;
struct CalculateExpression;
struct CompareExpression;
struct AssignExpression;

// statements
struct Statement;
struct ExpressionStatement;
struct BlockStatement;
struct IfStatement;
struct WhileStatement;
struct ForStatement;

struct Program;

// types
struct TypeAst;
struct IntegerTypeAst;
struct RealTypeAst;

// type references
struct TypeReference;
struct IntegerTypeReference;
struct RealTypeReference;

struct VariableDefinition;
struct ParameterDefinition;
using Argument = ParameterDefinition;
struct FunctionDefinition;
using ParameterList = std::vector<Pointer<ParameterDefinition> >;
using ArgumentList = std::vector<Pointer<Expression> >;

struct Symbol;
struct VariableSymbol;
struct FunctionSymbol;
struct ClassTypeSymbol;
struct BuiltInTypeSymbol;

struct IVisitor {
 public:
  virtual void Visit(NumberExpression* node) {}

  virtual void Visit(EmptyExpression* node) {}

  virtual void Visit(VariableExpression* node) {}

  virtual void Visit(CalculateExpression* node) {}

  virtual void Visit(CompareExpression* node) {}

  virtual void Visit(AssignExpression* node) {}

  virtual void Visit(ExpressionStatement* node) {}

  virtual void Visit(BlockStatement* node) {}

  virtual void Visit(IfStatement* node) {}

  virtual void Visit(WhileStatement* node) {}

  virtual void Visit(ForStatement* node) {}

  virtual void Visit(FunctionExpression* node) {}

  virtual void Visit(VariableDefinition* node) {}

  virtual void Visit(FunctionDefinition* node) {}

  virtual void Visit(Program* node) {}

  virtual void Visit(VariableSymbol *node) {}

  virtual void Visit(ClassTypeSymbol* node) {}

  virtual void Visit(BuiltInTypeSymbol* node) {}

  virtual void Visit(ParameterDefinition* node) {}

  virtual void Visit(FunctionSymbol *node) {}
};

struct IVisitable {
 public:
  virtual void Accept(IVisitor* visitor) = 0;
};

struct Ast : public IVisitable {
};

struct Expression : public Ast {
};

struct EmptyExpression : public Expression {
  void Accept(IVisitor* visitor) override { visitor->Visit(this); }
};

struct NumberExpression : public Expression {
  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Token token;
  double value;
};

struct VariableExpression : public Expression {
  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Token token;
  std::string name;
};

struct FunctionExpression : public Expression {
    void Accept(IVisitor* visitor) override { visitor->Visit(this); }

    std::string name;
    ArgumentList argument_list;
};

struct CalculateExpression : public Expression {
  enum struct Operator {
    kPlus,  // +
    kMinus,  // -
    kMultiply,  // *
    kDivide,  // /
  };

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Pointer<Expression> left_expression;
  Operator operatorr;
  Pointer<Expression> right_expression;
};

struct CompareExpression : public Expression {
  enum struct Operator {
    kGreater,
    kLesser,
  };

  void Accept(IVisitor* visitor) override {
    visitor->Visit(this);
  }

  Pointer<Expression> left_expression;
  Operator opeartor_;
  Pointer<Expression> right_expression;
};

struct AssignExpression : public Expression {
  void Accept(IVisitor* visitor) override {
    visitor->Visit(this);
  }

  Token operator_token;
  Pointer<Expression> left_expression;
  Pointer<Expression> right_expression;
};

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
