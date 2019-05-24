
#include <iostream>
#include <vegetable_script/mul.h>
#include <common/all.h>
#include <parsing/all.h>
#include <set>
#include <map>
#include <tuple>
#include <string>

void OutputTokens(vegetable_script::Lexer* lexer) {
  using namespace vegetable_script;  // NOLINT
  while (lexer->HasNext()) {
      ParsingException exception;
      Token token;
      if (!lexer->LookCurrentWithoutComments(&token, &exception)) {
        std::cout << "exception: " << exception.status << ", at("
          << exception.row << ", " << exception.column << ")\n";
        return;
      }
      lexer->MoveNext();
      std::cout << "next token:" << token.ToEscapedString() << std::endl;
    }
}


XC_TEST_CASE(LEXER, true) {
  std::cout << __FILE__ << std::endl;
  using namespace vegetable_script;  // NOLINT
  const char* source_codes[] = {
    R"DEF(
    "fdsfa"
    a
    //dfafds
    /*dd
    d (3+3*3_)

    //d*/
    "ddddd\""
*)DEF"
  };

  for (int i = 0; i < 1; ++i) {
    std::cout << "start test lexer of " << i << std::endl;
    std::string source = source_codes[i];
    auto source_provider = std::make_shared<SourceProvider>(source);
    Lexer lexer = Lexer(source_provider);
    OutputTokens(&lexer);
  }
}
