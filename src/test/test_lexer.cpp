
#include <iostream>
#include <vegetable_script/mul.h>
#include <common/all.h>
#include <parsing/all.h>
#include <set>
#include <map>
#include <tuple>
#include <string>


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
    Lexer lexer = Lexer(source_provider, true);
    while (lexer.HasNext()) {
      auto errors = Lexer::Error::List();
      auto token = lexer.LookCurrent(&errors);
      while (!errors.empty()) {
        Lexer::Error error = errors.front();
        errors.pop_front();
        std::cout << "error: " << error.status << ", at(" << error.row
            << ", " << error.column << ")\n";
      }
      lexer.MoveNext();
      std::cout << "next token:" << token->ToEscapedString() << std::endl;
    }
  }
}
