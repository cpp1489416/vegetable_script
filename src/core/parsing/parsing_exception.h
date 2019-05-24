#ifndef VEGETABLE_SCRIPT_CORE_PARSION_PARSING_ERROR_H_
#define VEGETABLE_SCRIPT_CORE_PARSION_PARSING_ERROR_H_

#include <string>
#include <deque>
#include <memory>
#include <vector>
#include "./source_provider.h"
#include "./token.h"
#include "../common/all.h"

namespace vegetable_script {

struct ParsingException {
  using Ptr = std::shared_ptr<ParsingException>;
  using List = std::deque<ParsingException>;

  std::string ToString();
  std::string ToString(const std::string& additional);

  std::string status;
  int row;
  int column;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSION_PARSING_ERROR_H_
