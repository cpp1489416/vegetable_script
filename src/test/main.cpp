
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

XC_TEST_CASE(TokenNull, false) {
  std::shared_ptr<int> i = nullptr;
  if (i) {
    std::cout << "not null";
  } else {
    std::cout << "null";
  }
  std::cout << "terfef";
}
