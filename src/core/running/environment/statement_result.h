#ifndef RANDOM_KJSE4NBXLXEE2WEHE2EF2QMD6IPZRUI0AOX8HBPKFFADEISWICISDUYXFJIS
#define RANDOM_KJSE4NBXLXEE2WEHE2EF2QMD6IPZRUI0AOX8HBPKFFADEISWICISDUYXFJIS

#include "./expression_result.h"

namespace vegetable_script {

struct StatementResult final {
  enum class Type {
    kNormal,
    kContinue,
    kBreak,
    kReturn,
  };

  StatementResult();
  explicit StatementResult(const ExpressionResult& expression_result);
  explicit StatementResult(Type type);
  explicit StatementResult(const ExpressionResult& expression_result, Type type);
  StatementResult(const StatementResult& rhs) = default;
  StatementResult& operator=(const StatementResult& rhs) = default;

  Type type;
  ExpressionResult expression_result;
};


}  // namespace vegetable_script

#endif  // RANDOM_KJSE4NBXLXEE2WEHE2EF2QMD6IPZRUI0AOX8HBPKFFADEISWICISDUYXFJIS
