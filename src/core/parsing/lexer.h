#ifndef VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_
#define VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_

#include <string>
#include <deque>
#include <memory>
#include <vector>
#include "./source_provider.h"
#include "./token.h"
#include "../common/all.h"

namespace vegetable_script {

namespace details {

struct LexerResultError {
  using Ptr = std::shared_ptr<LexerResultError>;
  using List = std::deque<LexerResultError>;

  std::string status;
  int row;
  int column;
};

}  // namespace details

class Lexer {
 public:
  using Ptr = std::shared_ptr<Lexer>;
  using Error = details::LexerResultError;

  explicit Lexer(
      SourceProvider::Ptr source_provider,
      bool on_error_skip = true);

  bool HasNext();

  void MoveNext();

  Token::Ptr LookCurrent(Error::List* errors);

  Token::Ptr LookCurrentWithoutComments(Error::List* errors);

  Token::Ptr LookAhead(Error::List* errors);

  Token::Ptr LookAheadWithoutComments(Error::List* errors);

  Token::Ptr LookAhead(int more, Error::List* errors);

  Token::Ptr LookAheadWithoutComments(int more, Error::List* errors);

  bool on_error_skip() const { return on_error_skip_; }

  bool set_on_error_skip(bool value) { on_error_skip_ = value; }

 private:
  void Epoch(Error::List* errors);

  void Epoch0(Error::List* errors);

  void EpochElse(int status, int row, int column,
      std::string* string, Error::List* errors);

  void SkipBlanks();

  void PushBackToken(const std::string& string,
      const Token::Type& type, int row, int column);

  void PushBackError(const std::string& status, int row, int column,
      Error::List* errors);

  SourceProvider::Ptr source_provider_;
  bool on_error_skip_;
  Token::PtrDeque tokens_;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_
