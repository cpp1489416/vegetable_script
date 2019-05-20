
#include <iostream>
#include <vegetable_script/mul.h>
#include <common/all.h>
#include <parsing/all.h>
#include <set>
#include <map>
#include <tuple>

#define ARGS(...) __VA_ARGS__

int main() {
  static int i = 3;
  return 0;
}

XC_MULTI_ENUM(
  Color,
  kRed,
  kBlue
);

XC_MULTI_ENUM(
  AColor,
  kRed,
  kBlue
);

XC_TEST_CASE(ENUM, false) {
  int ARGS(x, y, z) = 3;
  Color color = Color::kRed;
  std::cout << Color::TypeToString(Color::kRed) << std::endl;
  std::cout << Color::StringToType("kBlue") << std::endl;
  Color color2 = Color::kRed;
  auto types = color2.types();
  std::cout << (color == color2) << std::endl;
  color = Color();
  color = Color::kRed;
  color = Color::kBlue;
  color >> Color::kRed;
  color >> Color::kRed;
  std:: cout << "is color equals" << (color == Color(Color::kBlue))
    << std::endl;
  std::cout << color.ToString() << std::endl;
}

XC_TEST_CASE(ARFDS, false) {
}

XC_TEST_CASE(Token, true) {
  const char* source_code = R"DEF(
    var i = 1;
    var j = 4;
    var k = -4.3;
    if (i == 3) {
      return false;
    }
  )DEF";
  using namespace vegetable_script; // NOLINT
  Lexer l = Lexer(source_code);
  while (l.IsHasNext()) {
    Token t = l.GetNextToken();
    std::cout << "next token: " << t.ToString() << std::endl;
  }


  std::cout << source_code;
  std::cout << "eat shit !!!" << std::endl;
}
