
#include <iostream>
#include <common/all.h>
#include <parsing/all.h>
#include <set>
#include <map>
#include <tuple>
#include <string>


XC_TEST_CASE(FILE, false) {
  std::string directory = xc::FileUtil::GetDirectory(__FILE__);
  std::cout << __FILE__ << std::endl;
  std::cout << xc::FileUtil::GetDirectory(__FILE__) << std::endl;
  std::cout << xc::FileUtil::GetDirectory("/a.cpp") << std::endl;
  std::cout << xc::FileUtil::GetDirectory("a.cpp") << std::endl;
  std::cout << "read me: " <<
      xc::FileUtil::ReadToString(directory + "/sample/a.inc") << std::endl;
}