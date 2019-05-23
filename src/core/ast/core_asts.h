#ifndef VEGETABLE_SCRIPT_CORE_AST_CORE_ASTS_H_
#define VEGETABLE_SCRIPT_CORE_AST_CORE_ASTS_H_

#include <string>
#include <vector>
#include <memory>
#include "../parsing/token.h"

namespace vegetable_script {

struct IVisitable;

struct Ast;

// expressions
struct Expression;
struct EmptyExpression;
struct LHSExpression;
struct VariableExpression;
struct IdentifierExpression;
struct StringExpression;
struct NumberExpression;
struct IntegerExpression;
struct FloatExpression;
struct FunctionExpression;
struct UnaryExpression;
struct BinaryExpression;
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

struct Symbol;
struct VariableSymbol;
struct FunctionSymbol;
struct ClassTypeSymbol;
struct BuiltInTypeSymbol;

struct IVisitor {
 public:
  virtual void Visit(IdentifierExpression* node) {}

  virtual void Visit(StringExpression* node) {}

  virtual void Visit(NumberExpression* node) {}

  virtual void Visit(IntegerExpression* node) {}

  virtual void Visit(FloatExpression* node) {}

  virtual void Visit(EmptyExpression* node) {}

  virtual void Visit(VariableExpression* node) {}

  virtual void Visit(UnaryExpression* node) {}

  virtual void Visit(BinaryExpression* node) {}

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

struct Ast : public IVisitable, std::enable_shared_from_this<Ast> {
  using WeakPtr = std::weak_ptr<Ast>;
  using Ptr = std::shared_ptr<Ast>;

  WeakPtr parent;
};

struct Expression : public Ast {
  using Ptr = std::shared_ptr<Expression>;
  using WeakPtr = std::weak_ptr<Expression>;
};

struct IdentifierExpression : public Expression {
  using Ptr = std::shared_ptr<IdentifierExpression>;
  using WeakPtr = std::weak_ptr<IdentifierExpression>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  std::string value;
};

struct StringExpression : public Expression {
  using Ptr = std::shared_ptr<StringExpression>;
  using WeakPtr = std::weak_ptr<StringExpression>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  std::string value;
};

struct EmptyExpression : public Expression {
  using Ptr = std::shared_ptr<EmptyExpression>;
  using WeakPtr = std::weak_ptr<EmptyExpression>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }
};

struct NumberExpression : public Expression {
  using Ptr = std::shared_ptr<NumberExpression>;
  using WeakPtr = std::weak_ptr<NumberExpression>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }
};

struct IntegerExpression: public NumberExpression {
  using Ptr = std::shared_ptr<IntegerExpression>;
  using WeakPtr = std::weak_ptr<IntegerExpression>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  int value;
};

struct FloatExpression: public NumberExpression {
  using Ptr = std::shared_ptr<FloatExpression>;
  using WeakPtr = std::weak_ptr<FloatExpression>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  double value;
};

struct VariableExpression : public Expression {
  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Token token;
  std::string name;
};

struct FunctionExpression : public Expression {
  using ArgumentList = std::vector<Expression::Ptr>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  std::string name;
  ArgumentList argument_list;
};

struct UnaryExpression: public Expression {
  XC_MULTI_ENUM(
    Operator,
    kPositive,
    kNegative
  )

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Operator operatorr;
  Expression::Ptr child_expression;
};

struct BinaryExpression : public Expression {
  XC_MULTI_ENUM(
    Operator,
    kPlus,  // +
    kMinus,  // -
    kMultiply,  // *
    kDivide,  // /
    kGreater,  // >
    kLesser,  // <
    kEqual
  )

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Expression::Ptr left_expression;
  Operator operatorr;
  Expression::Ptr right_expression;
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
