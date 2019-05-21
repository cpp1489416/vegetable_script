#ifndef VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_
#define VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_

#include <string>
#include <deque>
#include <memory>
#include "./source_provider.h"
#include "./token.h"
#include "../common/all.h"

namespace vegetable_script {

class Lexer {
 public:
  explicit Lexer(SourceProvider* source_provider);

  bool HasNext();

  std::shared_ptr<Token> MoveNext();

  std::shared_ptr<Token> LookCurrent();

  std::shared_ptr<Token> LookAhead(int more = 1);

 private:
  void Epoch();

  void Epoch0();

  void EpochElse(int status, std::string* string, int row, int column);

  void SkipBlanks();

  void RaiseError();

  void PushBackToken(const std::string& string,
      const Token::Type& type, int row, int column);

  SourceProvider* source_provider_;
  std::deque<std::shared_ptr<Token> > tokens_;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_
