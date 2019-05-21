#ifndef VEGETABLE_SCRIPT_CORE_PARSING_LL1_EXPRESSION_ANALYZER_H_
#define VEGETABLE_SCRIPT_CORE_PARSING_LL1_EXPRESSION_ANALYZER_H_

#include "./lexer.h"

namespace vegetable_script {

class LL1ExpressionAnalyzer {
 public:
  explicit LL1ExpressionAnalyzer(Lexer::Ptr lexer);

  void a();

 private:
  Lexer::Ptr lexer_;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSING_LL1_EXPRESSION_ANALYZER_H_
