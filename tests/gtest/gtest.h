#ifndef PIXELVERSE_OFFLINE_GTEST_H
#define PIXELVERSE_OFFLINE_GTEST_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <chrono>
#include <cmath>
#include <sstream>
#include <cstddef>

namespace testing {

class Test {
public:
    virtual ~Test() = default;
    virtual void TestBody() = 0;

    static void RecordProperty(const std::string& key, const std::string& value);
};

struct TestInfo {
    std::string testSuiteName;
    std::string testName;
    std::function<Test*()> factory;
    bool passed = true;
    double durationMs = 0.0;
    std::vector<std::string> failureMessages;
};

class UnitTest {
private:
    std::vector<TestInfo> tests;
    static UnitTest* s_instance;

public:
    static UnitTest* GetInstance();

    void RegisterTest(const std::string& suiteName, const std::string& testName, std::function<Test*()> factory);
    int RunAllTests(const std::string& filter = "");
    const std::vector<TestInfo>& GetTests() const { return tests; }
    void ClearFailures();
};

template <typename T>
inline void PrintValue(std::ostream& os, const T& val) {
    os << val;
}

inline void PrintValue(std::ostream& os, std::nullptr_t) {
    os << "nullptr";
}

template <typename T>
inline void PrintValue(std::ostream& os, T* ptr) {
    if (ptr == nullptr) {
        os << "nullptr";
    } else {
        os << static_cast<const void*>(ptr);
    }
}

void CurrentTestFail(const std::string& msg);
bool IsCurrentTestFailed();

void InitGoogleTest(int* argc, char** argv);

} // namespace testing

#define GTEST_TEST_CLASS_NAME_(test_suite_name, test_name) \
  test_suite_name##_##test_name##_Test

#define TEST(test_suite_name, test_name) \
  class GTEST_TEST_CLASS_NAME_(test_suite_name, test_name) : public ::testing::Test { \
   public: \
    GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)() = default; \
    void TestBody() override; \
   private: \
    static const bool registered_; \
  }; \
  const bool GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::registered_ = []() { \
    ::testing::UnitTest::GetInstance()->RegisterTest( \
        #test_suite_name, #test_name, []() -> ::testing::Test* { \
          return new GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)(); \
        }); \
    return true; \
  }(); \
  void GTEST_TEST_CLASS_NAME_(test_suite_name, test_name)::TestBody()

#define EXPECT_TRUE(cond) \
  do { \
    if (!(cond)) { \
      ::testing::CurrentTestFail(std::string("Expected true: ") + #cond + " at " + __FILE__ + ":" + std::to_string(__LINE__)); \
    } \
  } while (0)

#define EXPECT_FALSE(cond) \
  do { \
    if (cond) { \
      ::testing::CurrentTestFail(std::string("Expected false: ") + #cond + " at " + __FILE__ + ":" + std::to_string(__LINE__)); \
    } \
  } while (0)

#define EXPECT_EQ(val1, val2) \
  do { \
    if (!((val1) == (val2))) { \
      std::ostringstream _ss; \
      _ss << "Expected equality of (" #val1 ") and (" #val2 "), but got "; \
      ::testing::PrintValue(_ss, (val1)); \
      _ss << " vs "; \
      ::testing::PrintValue(_ss, (val2)); \
      _ss << " at " << __FILE__ << ":" << __LINE__; \
      ::testing::CurrentTestFail(_ss.str()); \
    } \
  } while (0)

#define EXPECT_NE(val1, val2) \
  do { \
    if ((val1) == (val2)) { \
      std::ostringstream _ss; \
      _ss << "Expected inequality of (" #val1 ") and (" #val2 "), but both are "; \
      ::testing::PrintValue(_ss, (val1)); \
      _ss << " at " << __FILE__ << ":" << __LINE__; \
      ::testing::CurrentTestFail(_ss.str()); \
    } \
  } while (0)

#define EXPECT_GT(val1, val2) \
  do { \
    if (!((val1) > (val2))) { \
      std::ostringstream _ss; \
      _ss << "Expected (" #val1 " > " #val2 "), but "; \
      ::testing::PrintValue(_ss, (val1)); \
      _ss << " <= "; \
      ::testing::PrintValue(_ss, (val2)); \
      _ss << " at " << __FILE__ << ":" << __LINE__; \
      ::testing::CurrentTestFail(_ss.str()); \
    } \
  } while (0)

#define EXPECT_GE(val1, val2) \
  do { \
    if (!((val1) >= (val2))) { \
      std::ostringstream _ss; \
      _ss << "Expected (" #val1 " >= " #val2 "), but "; \
      ::testing::PrintValue(_ss, (val1)); \
      _ss << " < "; \
      ::testing::PrintValue(_ss, (val2)); \
      _ss << " at " << __FILE__ << ":" << __LINE__; \
      ::testing::CurrentTestFail(_ss.str()); \
    } \
  } while (0)

#define EXPECT_LT(val1, val2) \
  do { \
    if (!((val1) < (val2))) { \
      std::ostringstream _ss; \
      _ss << "Expected (" #val1 " < " #val2 "), but "; \
      ::testing::PrintValue(_ss, (val1)); \
      _ss << " >= "; \
      ::testing::PrintValue(_ss, (val2)); \
      _ss << " at " << __FILE__ << ":" << __LINE__; \
      ::testing::CurrentTestFail(_ss.str()); \
    } \
  } while (0)

#define EXPECT_LE(val1, val2) \
  do { \
    if (!((val1) <= (val2))) { \
      std::ostringstream _ss; \
      _ss << "Expected (" #val1 " <= " #val2 "), but "; \
      ::testing::PrintValue(_ss, (val1)); \
      _ss << " > "; \
      ::testing::PrintValue(_ss, (val2)); \
      _ss << " at " << __FILE__ << ":" << __LINE__; \
      ::testing::CurrentTestFail(_ss.str()); \
    } \
  } while (0)

#define EXPECT_NEAR(val1, val2, abs_error) \
  do { \
    if (std::abs((val1) - (val2)) > (abs_error)) { \
      std::ostringstream _ss; \
      _ss << "Expected near (" #val1 ") and (" #val2 ") within " << (abs_error) << ", diff is " << std::abs((val1) - (val2)) << " at " << __FILE__ << ":" << __LINE__; \
      ::testing::CurrentTestFail(_ss.str()); \
    } \
  } while (0)

#define ASSERT_TRUE(cond) EXPECT_TRUE(cond); if (::testing::IsCurrentTestFailed()) return
#define ASSERT_FALSE(cond) EXPECT_FALSE(cond); if (::testing::IsCurrentTestFailed()) return
#define ASSERT_EQ(val1, val2) EXPECT_EQ(val1, val2); if (::testing::IsCurrentTestFailed()) return
#define ASSERT_NE(val1, val2) EXPECT_NE(val1, val2); if (::testing::IsCurrentTestFailed()) return
#define ASSERT_GE(val1, val2) EXPECT_GE(val1, val2); if (::testing::IsCurrentTestFailed()) return

#define RUN_ALL_TESTS() ::testing::UnitTest::GetInstance()->RunAllTests()

#endif // PIXELVERSE_OFFLINE_GTEST_H
