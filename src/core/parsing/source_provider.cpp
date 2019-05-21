
#include "./source_provider.h"

namespace vegetable_script {

SourceProvider::SourceProvider(const std::string& source):
    source_(source), position_(0), row_(1), column_(1) {
}

char SourceProvider::LookCurrent() {
    if (position_ >= source_.length()) {
        return '\0';
    }
    return source_.at(position_);
}

int SourceProvider::LookRow() {
    return row_;
}

int SourceProvider::LookColumn() {
    return column_;
}

char SourceProvider::LookAhead(int count) {
    int pos = position_ + count;
    if (pos >= source_.size()) {
        return '\0';
    }
    return source_.at(pos);
}

bool SourceProvider::HasNext() {
    return LookAhead() != '\0';
}

char SourceProvider::MoveNext() {
    ++position_;
    char c = LookCurrent();
    if (c == '\n') {
        ++row_;
        column_ = 1;
    } else {
        ++column_;
    }
    return c;
}

}  // namespace vegetable_script
