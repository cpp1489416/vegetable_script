#ifndef XC_MULTI_ENUM_H_
#define XC_MULTI_ENUM_H_

#include <string>
#include <set>
#include <map>
#include <tuple>

#define XC_MULTI_ENUM_STRINGIFY(x) #x

#define XC_MULTI_ENUM_TRAIT_FIRST(x, ...) x

#define XC_MULTI_ENUM(ENUM_NAME, ...)                                 \
  class ENUM_NAME {                                                   \
   public:                                                            \
    enum Type_ {                                                      \
      __VA_ARGS__                                                     \
    };                                                                \
                                                                      \
    ENUM_NAME() {                                                     \
    }                                                                 \
                                                                      \
    ENUM_NAME(Type_ type) {  /* NOLINT(runtime/explicit) */           \
      types_.insert(type);                                            \
    }                                                                 \
                                                                      \
    ENUM_NAME(const ENUM_NAME& rhs) {                                 \
       types_ = rhs.types_;                                           \
                                                                      \
    }                                                                 \
                                                                      \
    ENUM_NAME& operator=(const Type_& rhs) {                          \
      types_.insert(rhs);                                             \
    }                                                                 \
                                                                      \
    ENUM_NAME& operator=(const ENUM_NAME& rhs) {                      \
      types_ = rhs.types_;                                            \
    }                                                                 \
                                                                      \
    bool operator==(const ENUM_NAME& rhs) const {                     \
      return types_ == rhs.types_;                                    \
    }                                                                 \
                                                                      \
    bool operator==(const Type_& rhs) const {                         \
      return types_.count(rhs) != 0;                                  \
    }                                                                 \
                                                                      \
    bool operator!=(const ENUM_NAME& rhs) const {                     \
      return !(*this == rhs);                                         \
    }                                                                 \
                                                                      \
    bool operator!=(const Type_& rhs) const {                         \
      return !(*this == rhs);                                         \
    }                                                                 \
                                                                      \
    ENUM_NAME& operator<<(const Type_& rhs) {                         \
      types_.insert(rhs);                                             \
      return *this;                                                   \
    }                                                                 \
                                                                      \
    ENUM_NAME& operator>>(const Type_& rhs) {                         \
      types_.erase(rhs);                                              \
      return *this;                                                   \
    }                                                                 \
                                                                      \
    static std::string TypeToString(Type_ type) {                     \
      auto type_strings =                                             \
        std::get<0>(GenerateTypeStringsAndStringTypes());             \
      return type_strings->at(type);                                  \
    }                                                                 \
                                                                      \
    static Type_ StringToType(const std::string &string) {            \
      auto string_types =                                             \
        std::get<1>(GenerateTypeStringsAndStringTypes());             \
      return string_types->at(string);                                \
    }                                                                 \
                                                                      \
    std::string ToString() const {                                    \
      std::string ans = "MultiEnum(";                                 \
      auto itr = types_.cbegin();                                     \
      if (itr != types_.cend()) {                                     \
        ans += TypeToString(*itr);                                    \
        for (++itr; itr != types_.cend(); ++itr) {                    \
          ans += ", ";                                                \
          ans += TypeToString(*itr);                                  \
        }                                                             \
      }                                                               \
      ans += ")";                                                     \
      return ans;                                                     \
    }                                                                 \
                                                                      \
    const std::set<Type_> types() const {                             \
      return types_;                                                  \
    }                                                                 \
                                                                      \
   private:                                                           \
    static std::tuple<const std::map<Type_, std::string>*,            \
      const std::map<std::string, Type_>* >                           \
      GenerateTypeStringsAndStringTypes() {                           \
      static std::map<Type_, std::string> type_strings;               \
      static std::map<std::string, Type_> string_types;               \
      if (type_strings.size() == 0) {                                 \
        std::string source = XC_MULTI_ENUM_STRINGIFY((__VA_ARGS__));  \
        int start_index =                                             \
          static_cast<int>(XC_MULTI_ENUM_TRAIT_FIRST(__VA_ARGS__));   \
          char brackets_array[] = { ' ', '(', ')' };                  \
          std::set<char> brackets(brackets_array,                     \
            brackets_array + 3);                                      \
          source = xc::StringUtil::RemoveChars(source, brackets);     \
          std::vector<std::string> strs =                             \
            xc::StringUtil::Split(source, ',');                       \
          for (size_t i = 0; i < strs.size(); ++i) {                  \
            type_strings[static_cast<Type_>(start_index + i)] =       \
              strs[i];                                                \
            string_types[strs[i]] =                                   \
              static_cast<Type_>(start_index + i);                    \
          }                                                           \
      }                                                               \
      return std::make_tuple(&type_strings, &string_types);           \
    }                                                                 \
    std::set<Type_> types_;                                           \
  };

#endif  // XC_MULTI_ENUM_H_
