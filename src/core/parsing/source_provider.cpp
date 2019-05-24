#include "./source_provider.h"
#include "../common/file_util.h"

namespace vegetable_script {

SourceProvider::SourceProvider(const std::string& source):
  source_(source), position_(0), row_(1), column_(1) {
}

SourceProvider::SourceProvider(const SourceProvider& rhs) :
  source_(rhs.source_),
  position_(rhs.position_),
  row_(rhs.row_),
  column_(rhs.column_) {
}

SourceProvider SourceProvider::FromFile(const std::string& path) {
  SourceProvider ans = SourceProvider(xc::FileUtil::ReadToString(path));
  return ans;
}

char SourceProvider::LookCurrent() {
  if (position_ >= source_.length()) {
    return '\0';
  }
  switch (GetEnterType()) {
    case EnterType::kN:
    case EnterType::kNotEnter:
      return source_[position_];
    case EnterType::kR:
      return '\n';
    case EnterType::kRN:
      ++position_;
      return '\n';
  }
}

int SourceProvider::LookRow() {
  return row_;
}

int SourceProvider::LookColumn() {
  return column_;
}

char SourceProvider::LookAhead(int count) {
  int pos = position_;
  while (count--) {
    switch (GetEnterType()) {
      case EnterType::kRN:
        pos += 2;
      default:
        ++pos;
    }
    if (pos >= source_.length()) {
      return '\0';
    }
  }
  return source_[pos];
}

bool SourceProvider::HasNext() {
    return LookCurrent() != '\0';
}

void SourceProvider::MoveNext() {
    char c = LookCurrent();
    switch (GetEnterType()) {
      case EnterType::kNotEnter:
        ++position_;
        ++column_;
        return;
      case EnterType::kRN:
        ++position_;
      case EnterType::kR:
      case EnterType::kN:
        ++position_;
        ++row_;
        column_ = 1;
        return;
    }
}

SourceProvider::EnterType SourceProvider::GetEnterType() {
  if (position_ >= source_.length()) {
    return EnterType::kNotEnter;
  }
  char c = source_[position_];
  if (c == '\n') {
    return EnterType::kN;
  } else if (c == '\r') {
    if (position_ + 1 >= source_.length() || source_[position_ + 1] != '\n') {
      return EnterType::kR;
    } else {
      return EnterType::kRN;
    }
  } else {
    return EnterType::kNotEnter;
  }
}

}  // namespace vegetable_script
