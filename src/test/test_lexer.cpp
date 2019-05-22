
#include <iostream>
#include <vegetable_script/mul.h>
#include <common/all.h>
#include <parsing/all.h>
#include <set>
#include <map>
#include <tuple>
#include <string>

XC_TEST_CASE(LEXER, true) {
  std::cout << "moving";
  using namespace vegetable_script;  // NOLINT
  const char* source_code = R"DEF(
    ;
    ;
    ;
    1
    2
    44
    3242342
    re34234234324
    +
    -
    *
    * /
    -.342423
    34324.33
    +3
    /*
    fdsaf
    sa
    fd
    sad*/
    //
    f | |||& && & &
    "stri\dng"
    ()
  )DEF";
  std::cout << "moving";
  auto source_provider = std::make_shared<SourceProvider>(source_code);
  Lexer lexer = Lexer(source_provider);
  std::cout << "moving";
  while (lexer.HasNext()) {
	  auto result = lexer.LookCurrent();

	  std::cout << result->token->ToString() << std::endl;
	  lexer.MoveNext();
  }
  system("Pause");
  return;
  std::cout << "eat" << std::endl;
}
