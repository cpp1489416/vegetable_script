#ifndef VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_
#define VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_

#include <string>
#include <deque>
#include <memory>
#include "./source_provider.h"
#include "./token.h"
#include "../common/all.h"

namespace vegetable_script {

namespace detail {

struct LexerResultError {
  using Ptr = std::shared_ptr<LexerResultError>;

  int row;
  int column;
};

struct LexerResult {
  using Ptr = std::shared_ptr<LexerResult>;
  using Error = LexerResultError;
  using List = std::deque<Ptr>;

  Token::Ptr token;
  Error::Ptr error;
};

}  // namespace detail

class Lexer {
 public:
  using Ptr = std::shared_ptr<Lexer>;
  using Result = detail::LexerResult;

  explicit Lexer(SourceProvider::Ptr source_provider);

  bool HasNext();

  Result::Ptr MoveNext();

  Result::Ptr LookCurrent();

  Result::Ptr LookAhead(int more = 1);

 private:
  void Epoch();

  void Epoch0();

  void EpochElse(int status, std::string* string, int row, int column);

  void SkipBlanks();

  void PushBackToken(const std::string& string,
      const Token::Type& type, int row, int column);

  void PushBackError(int row, int column);

  SourceProvider::Ptr source_provider_;
  Result::List results_;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_
