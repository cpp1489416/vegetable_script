#ifndef VEGETABLE_SCRIPT_CORE_AST_EXPRESSION_ASTS_H_
#define VEGETABLE_SCRIPT_CORE_AST_EXPRESSION_ASTS_H_

#include <string>
#include <vector>
#include <memory>
#include "../parsing/token.h"
#include "./ast_declarations.h"

namespace vegetable_script  {

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

struct UnaryExpression: public Expression {
  using Ptr = std::shared_ptr<UnaryExpression>;

  XC_MULTI_ENUM(
    Operator,
    kPositive,
    kNegative
  )

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Operator operatorr;
  Expression::Ptr operand_expression;
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
    kEqual,
    kComma,
  )

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  Expression::Ptr left_expression;
  Operator operatorr;
  Expression::Ptr right_expression;
};

struct FunctionInvokeExpression : public Expression {
  using Ptr = std::shared_ptr<FunctionInvokeExpression>;
  using ArgumentList = std::vector<Expression::Ptr>;

  void Accept(IVisitor* visitor) override { visitor->Visit(this); }

  std::string function_name;
  ArgumentList arguments;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_AST_EXPRESSION_ASTS_H_
