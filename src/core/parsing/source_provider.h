#ifndef VEGETABLE_SCRIPT_CORE_PARSING_SOURCE_PROVIDER_H_
#define VEGETABLE_SCRIPT_CORE_PARSING_SOURCE_PROVIDER_H_

#include <string>
#include <memory>

namespace vegetable_script {

namespace details {

}

class SourceProvider {
 public:
  enum class EnterType {
    kN, kRN, kR, kNotEnter,
  };

  using Ptr = std::shared_ptr<SourceProvider>;

  explicit SourceProvider(const std::string& source);

  SourceProvider(const SourceProvider& rhs);

  char LookCurrent();

  int LookRow();

  int LookColumn();

  inline char LookAhead() { return LookAhead(1); }

  char LookAhead(int count);

  bool HasNext();

  void MoveNext();

 private:
  EnterType GetEnterType();

  std::string source_;

  int position_;

  int row_;

  int column_;
};

}  // namespace vegetable_script

#endif  // VEGETABLE_SCRIPT_CORE_PARSING_SOURCE_PROVIDER_H
