#pragma once

#include <iostream>
#include <sstream>
#include "../ast/core_asts.h"

namespace vegetable_script {
class AstStringer : public IVisitor {
 public:
  void Visit(FloatExpression* node) override;

  void Visit(IntegerExpression* node) override;

  void Visit(StringExpression* node) override;

  void Visit(IdentifierExpression* node) override;

  void Visit(UnaryExpression *node) override;

  void Visit(BinaryExpression *node) override;

  void Visit(FunctionInvokeExpression* node) override;

  void Visit(ExpressionStatement* node) override;

  void Visit(BlockStatement* node) override;

  void Visit(IfStatement* node) override;

  void Visit(WhileStatement* node) override;

  void Visit(ForStatement* node) override;

  std::string Result() {
    return stream_.str();
  }

 private:
    std::stringstream stream_;
};

}  // namespace vegetable_script
