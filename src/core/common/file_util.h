
#ifndef XC_UTIL_FILE_UTIL_H_
#define XC_UTIL_FILE_UTIL_H_

#include <string>
#include <vector>
#include <set>

namespace xc {

class FileUtil {
 public:
  static std::string GetDirectory(const std::string& path);

  static std::string ReadToString(const std::string& path);
};

}  // namespace xc

#endif  // XC_UTIL_FILE_UTIL_H_
