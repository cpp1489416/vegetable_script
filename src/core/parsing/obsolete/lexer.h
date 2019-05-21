#ifndef VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_
#define VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_

#include <string>
#include <vector>
#include "./Token.h"

namespace vegetable_script {

class Lexer {
 public:
  explicit Lexer(std::string source_code) {
    source_code_ = source_code;
    Solve();
  }

  bool IsHasNext() const;

  void MoveToFront();

  Token GetNextToken();

  Token GetPreviousToken();

  Token LookAhead(int more = 1);

 private:
  void Solve();

  void AddString(std::string s);

  Token ConvertStringToToken(std::string s);

  std::vector<Token> token_array_;
  std::string source_code_;
  int current_index_ = 0;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_
