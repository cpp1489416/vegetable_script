#include "./functions.h"
#include "./scope_stack.h"
#include "../execute/double_statement_invoker.h"
#include <iostream>
#include <cmath>
#include <string>

namespace vegetable_script {

namespace {

class PrintFunction : public NativeFunctionSymbol {
 public:
  bool Invoke(ArgumentList* argument_list, ScopeStack* scope_stack, ReturnResult* result) override {
    for (auto a : *argument_list) {
      Argument argument = a;
      while (argument.value_type == Argument::ValueType::kIdentifier) {  // if it is a left value, change it to right value
        argument = scope_stack->scope()->GetVariable(argument.value.string_value);
      }
      switch (argument.value_type) {
        case Argument::ValueType::kVoid: {
          std::cout << "(void)";
          break;
        }
        case Argument::ValueType::kBool: {
          std::cout << "(bool)" << (argument.value.bool_value ? "true" : "false");
          break;
        }
        case Argument::ValueType::kInt: {
          std::cout << "(int)" << argument.value.int_value;
          break;
        }
        case Argument::ValueType::kFloat: {
          std::cout << "(float)" << argument.value.float_value;
          break;
        }
        case Argument::ValueType::kString: {
          std::cout << "(string)" << argument.value.string_value;
          break;
        }
        default: {
          std::cout << "strange";
        }
      }
      std::cout << " ";
    }
    std::cout << std::endl;
    *result = ReturnResult(ReturnResult::Type::kReturn);
    return true;
  }

  std::string GetName() override {
    return "print";
  }
};

class ReadIntegerFunction : public NativeFunctionSymbol {
 public:
  bool Invoke(ArgumentList* argument_list, ScopeStack* scope_stack, ReturnResult* result) override {
    int ans;
    std::cin >> ans;
    *result = ReturnResult(ExpressionResult(ans), ReturnResult::Type::kReturn);
    return true;
  }

  std::string GetName() override {
    return "read_int";
  }
};

class SinFunction : public NativeFunctionSymbol {
 public:
  bool Invoke(ArgumentList* argument_list, ScopeStack* scope_stack, ReturnResult* result) override {
    if (argument_list->size() != 1) {
      return false;
    }
    Argument argument = argument_list->front();
    switch (argument.value_type) {
      case Argument::ValueType::kVoid: {
        return false;
      }
      case Argument::ValueType::kBool: {
        return false;
      }
      case Argument::ValueType::kInt: {
        *result = ReturnResult(ExpressionResult(std::sin(argument.value.int_value)),
            ReturnResult::Type::kReturn);
        break;
      }
      case Argument::ValueType::kFloat: {
        *result = ReturnResult(ExpressionResult(std::sin(argument.value.float_value)),
            ReturnResult::Type::kReturn);
        break;
      }
      case Argument::ValueType::kString: {
        return false;
      }
    }
    return true;
  }

  std::string GetName() override {
    return "sin";
  }
};

class GetTypeFunction : public NativeFunctionSymbol {
 public:
  bool Invoke(ArgumentList* argument_list, ScopeStack* scope_stack, ReturnResult* result) override {
    if (argument_list->size() != 1) {
      return false;
    }
    Argument argument = argument_list->front();
    ExpressionResult expression_result;
    switch (argument.value_type) {
      case Argument::ValueType::kVoid: {
        expression_result = ExpressionResult("void");
        break;
      }
      case Argument::ValueType::kBool: {
        expression_result = ExpressionResult("bool");
        break;
      }
      case Argument::ValueType::kInt: {
        expression_result = ExpressionResult("int");
        break;
      }
      case Argument::ValueType::kFloat: {
        expression_result = ExpressionResult("float");
        break;
      }
      case Argument::ValueType::kString: {
        expression_result = ExpressionResult("string");
        break;
      }
    }
    *result = ReturnResult(expression_result, ReturnResult::Type::kReturn);
    return true;
  }

  std::string GetName() override {
    return "get_type";
  }
};

class CustomFunctionAstScopeStackFiller : public IVisitor {
 public:
  explicit CustomFunctionAstScopeStackFiller(ScopeStack* scope_stack) : scope_stack_(scope_stack) {}

  void Visit(BlockStatement* node) override {
    for (auto statement : node->statements) {
      statement->Accept(this);
    }
  }

  void Visit(FunctionDefinition* node) override {
    FunctionDefinition::Ptr node_ptr = std::static_pointer_cast<FunctionDefinition>(node->shared_from_this());
    scope_stack_->scope()->PutFunctionSymbol(std::make_shared<CustomFunctionSymbol>(node_ptr));
  }

 private:
  ScopeStack* scope_stack_;
};

}  // namespace

void NativeFunctionSymbol::FillScopeStack(ScopeStack* scope_stack) {
  scope_stack->scope()->PutFunctionSymbol(std::make_shared<PrintFunction>());
  scope_stack->scope()->PutFunctionSymbol(std::make_shared<ReadIntegerFunction>());
  scope_stack->scope()->PutFunctionSymbol(std::make_shared<SinFunction>());
  scope_stack->scope()->PutFunctionSymbol(std::make_shared<GetTypeFunction>());
}

CustomFunctionSymbol::CustomFunctionSymbol(FunctionDefinition::Ptr function_definition)
    : definition_(function_definition) { }

bool CustomFunctionSymbol::Invoke(
    CustomFunctionSymbol::ArgumentList* argument_list,
    ScopeStack* scope_stack,
    CustomFunctionSymbol::ReturnResult* result) {
  scope_stack->PushScope();
  for (size_t i = 0; i < argument_list->size(); ++i) {
    std::string name = definition_->parameter_list[i]->name;
    Argument argument;
    argument_list->at(i).ToRightValue(scope_stack, &argument);
    scope_stack->scope()->PutVariable(name, argument);
  }
  DoubleStatementInvoker statement_invoker(scope_stack);
  definition_->block_statement->Accept(&statement_invoker);
  bool ans = statement_invoker.success();
  *result = statement_invoker.result();
  scope_stack->PopScope();
  return true;
}

std::string CustomFunctionSymbol::GetName() {
  return definition_->name;
}

void CustomFunctionSymbol::FillScopeStack(Ast* ast, ScopeStack* scope_stack) {
  CustomFunctionAstScopeStackFiller filler(scope_stack);
  ast->Accept(&filler);
}

}  // namespace vegetable_script
