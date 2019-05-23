
#include "./file_util.h"
#include "./unit_test.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

namespace xc {

std::string FileUtil::GetDirectory(const std::string& path) {
  for (auto pos = static_cast<int>(path.length() - 1); pos >= 0; --pos) {
    if (path[pos] == '/' || path[pos] == '\\') {
      return path.substr(0, pos + 1);
    }
  }
  return "./";
}

std::string FileUtil::ReadToString(const std::string& path) {
  std::ifstream file;
  std::stringstream ans;
  file.open(path);
  if (!file) {
    file.close();
    ans << "no such file";
  } else {
    ans << file.rdbuf();
  }
  file.close();
  return ans.str();
}

}  // namespace xc
