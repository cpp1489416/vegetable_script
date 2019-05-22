
#ifndef XC_UTIL_STRING_UTIL_H_
#define XC_UTIL_STRING_UTIL_H_

#include <string>
#include <vector>
#include <set>

namespace xc {

class StringUtil {
 public:
  static std::string RemoveChars(const std::string& string,
                          const std::set<char>& chars);

  static std::vector<std::string> Split(const std::string& string,
                                 char source);

  static std::string Trim(const std::string& string);

  static int ToInt(const std::string& string);

  static double ToDouble(const std::string& string);
};

}  // namespace xc

#endif  // XC_UTIL_STRING_UTIL_H_
