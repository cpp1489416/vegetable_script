#pragma once

#include <iostream>
#include <sstream>
#include "../ast/core_asts.h"

namespace vegetable_script {
class AstStringer : public IVisitor {
 public:
  void Visit(FloatExpression* node) override;

  void Visit(IntegerExpression* node) override;

  void Visit(IdentifierExpression* node) override;

  void Visit(UnaryExpression *node) override;

  void Visit(BinaryExpression *node) override;

  std::string Result() {
    return stream_.str();
  }

 private:
    std::stringstream stream_;
};

}  // namespace vegetable_script
