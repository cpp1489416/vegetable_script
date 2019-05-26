#ifndef VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_
#define VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_

#include <string>
#include <deque>
#include <memory>
#include <vector>
#include "./source_provider.h"
#include "./token.h"
#include "../common/all.h"
#include "./parsing_exception.h"

namespace vegetable_script {

class Lexer {
 public:
  using Self = Lexer;
  using Ptr = std::shared_ptr<Self>;
  using Iterator = Self;

  explicit Lexer(SourceProvider::Ptr source_provider);

  bool HasNext();
  void MoveNext();

  bool LookCurrent(Token* token, ParsingException* exception);
  bool LookCurrentWithoutComments(Token* token, ParsingException* exception);
  bool LookAhead(Token* token, ParsingException* exception);
  bool LookAheadWithoutComments(Token* token, ParsingException* exception);
  bool LookAhead(size_t more, Token* token, ParsingException* exception);
  bool LookAheadWithoutComments(size_t more,
      Token* token, ParsingException* exception);

 private:
  bool Epoch(ParsingException* exception);
  bool Epoch0(ParsingException* exception);
  bool EpochElse(int status, int row, int column,
      std::string* string, ParsingException* exception);

  void SkipBlanks();
  void PushBackToken(const std::string& string, const Token::Type& type,
      int row, int column);
  void PushBackIdentifierOrKeyword(const std::string& string,
      int row, int column);

  SourceProvider::Ptr source_provider_;
  Token::Deque tokens_;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSION_LEXER_H_
