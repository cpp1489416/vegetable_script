#include "./parsing_exception.h"
#include <string>
#include "../common/string_util.h"

namespace vegetable_script {

std::string ParsingException::ToString() {
  return std::string() + "\033[31mparsing error at(" +
      std::to_string(row) + ", " + std::to_string(column) + ")\033[0m: " +
      status;
}

std::string ParsingException::ToString(const std::string& additional) {
  return ToString() + ", " + additional;
}

}  // namespace vegetable_script
