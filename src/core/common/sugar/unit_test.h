
#ifndef XC_TEST_CASE_FLAG
#define XC_TEST_CASE_FLAG


#define XC_TEST_CASE(name, opened)                                    \
  static void xc_test_case_##name();                                  \
  namespace xc_test_case {                                            \
  class TestCaseRunner##name {                                        \
   public:                                                            \
    TestCaseRunner##name() {                                          \
      if (!opened) {                                                  \
        return;                                                       \
      }                                                               \
      std::cout << "\033[30m\033[42m[start test case of "             \
        << #name << "]\033[0m\n";                                     \
      xc_test_case_##name();                                          \
      std::cout << "\033[30m\033[42m[end test case of "               \
        << #name << "]\033[0m\n";                                     \
    }                                                                 \
  };                                                                  \
  static TestCaseRunner##name gTestCase##name##Instance;              \
  }  /* namespace xc_test_case */                                     \
                                                                      \
  static void xc_test_case_##name()                                   \

#define XC_TEST_ASSERT(experssion)                                    \
  do {                                                                \
    if (!(experssion)) {                                              \
      throw "error when unit test ## \"expression\"";                 \
    }                                                                 \
  } while (0)

#endif  // XC_TEST_CASE_FLAG

