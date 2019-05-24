
#include "./string_util.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

namespace xc {

std::string StringUtil::RemoveChars(const std::string& string,
                          const std::set<char>& chars) {
  std::string ans;
  for (auto itr = string.cbegin(); itr != string.cend(); ++itr) {
    if (chars.count(*itr) == 0) {
      ans += *itr;
    }
  }
  return ans;
}

std::vector<std::string> StringUtil::Split(const std::string& string,
                                           char source) {
  std::vector<std::string> ans;
  size_t start_index = 0;
  size_t tmp_size = 0;
  for (auto itr = string.cbegin(); itr != string.cend(); ++itr) {
    if (*itr == source) {
      ans.push_back(string.substr(start_index, tmp_size));
      start_index += tmp_size + 1;
      tmp_size = 0;
    } else {
      ++tmp_size;
    }
  }
  ans.push_back(string.substr(start_index, tmp_size));
  return ans;
}

std::string StringUtil::Trim(const std::string& string) {
  if (string.size() == 0) {
    return "";
  }
  std::string ans = string;
  size_t start_index = string.size();
  size_t end_index = string.size();
  char ignored_array[] = {' ', '\n', '\r', '\t' };
  std::set<char> ignored(ignored_array, ignored_array + 4);
  for (size_t i = 0; i < string.size(); ++i) {
    char c = string.at(i);
    if (ignored.count(c) == 0) {
      start_index = i;
      break;
    }
  }
  for (size_t i = string.size() - 1; i >= 0 && i >= start_index; --i) {
    char c = string.at(i);
    if (ignored.count(c) == 0) {
      end_index = i + 1;
      break;
    }
  }
  return string.substr(start_index, end_index - start_index);
}

int StringUtil::ToInt(const std::string& string) {
  using namespace std;  // NOLINT
  return atoi(string.c_str());
}

double StringUtil::ToDouble(const std::string& string) {
  using namespace std;  // NOLINT
  return atof(string.c_str());
}

std::string StringUtil::Escape(const std::string& string) {
  std::stringstream string_stream;
  for (auto c : string) {
    switch (c) {
      case '\t': {
        string_stream << "\\t";
        break;
      }
      case '\r': {
        string_stream << "\\r";
        break;
      }
      case '\n': {
        string_stream << "\\n";
        break;
      }
      case '"': {
        string_stream << "\\\"";
        break;
      }
      default: {
        string_stream << c;
      }
    }
  }
  return string_stream.str();
}

}  // namespace xc
