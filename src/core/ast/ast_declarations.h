#ifndef VEGETABLE_SCRIPT_CORE_AST_AST_DECLARATIONS_H_
#define VEGETABLE_SCRIPT_CORE_AST_AST_DECLARATIONS_H_

namespace vegetable_script {

struct IVisitable;

struct Ast;

// expressions
struct Expression;
struct EmptyExpression;
struct LHSExpression;
struct IdentifierExpression;
struct StringExpression;
struct NumberExpression;
struct IntegerExpression;
struct FloatExpression;
struct FunctionInvokeExpression;
struct UnaryExpression;
struct BinaryExpression;

// statements
struct Statement;
struct EmptyStatement;
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
  virtual void Visit(Ast* node) {}
  virtual void Visit(IdentifierExpression* node) {}
  virtual void Visit(StringExpression* node) {}
  virtual void Visit(NumberExpression* node) {}
  virtual void Visit(IntegerExpression* node) {}
  virtual void Visit(FloatExpression* node) {}
  virtual void Visit(EmptyExpression* node) {}
  virtual void Visit(FunctionInvokeExpression* node) {}
  virtual void Visit(UnaryExpression* node) {}
  virtual void Visit(BinaryExpression* node) {}

  virtual void Visit(Statement* node) {}
  virtual void Visit(EmptyStatement* node) {}
  virtual void Visit(ExpressionStatement* node) {}
  virtual void Visit(BlockStatement* node) {}
  virtual void Visit(IfStatement* node) {}
  virtual void Visit(WhileStatement* node) {}
  virtual void Visit(ForStatement* node) {}
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
}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_AST_AST_DECLARATIONS_H_
