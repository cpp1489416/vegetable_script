#include "./statement_result.h"

namespace vegetable_script {

StatementResult::StatementResult() : type(Type::kNormal) {}

StatementResult::StatementResult(const ExpressionResult& expression_result)
    : expression_result(expression_result), type(Type::kNormal) {}

StatementResult::StatementResult(StatementResult::Type type)
    : type(type) {}

StatementResult::StatementResult(const ExpressionResult& expression_result, Type type)
    : expression_result(expression_result), type(type) {}

}  // namespace vegetable_script

