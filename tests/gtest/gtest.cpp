#include "gtest.h"
#include <iomanip>

namespace testing {

UnitTest* UnitTest::s_instance = nullptr;
static bool s_currentTestFailed = false;
static std::vector<std::string> s_currentTestErrors;

UnitTest* UnitTest::GetInstance() {
    if (!s_instance) {
        s_instance = new UnitTest();
    }
    return s_instance;
}

void CurrentTestFail(const std::string& msg) {
    s_currentTestFailed = true;
    s_currentTestErrors.push_back(msg);
}

bool IsCurrentTestFailed() {
    return s_currentTestFailed;
}

void UnitTest::RegisterTest(const std::string& suiteName, const std::string& testName, std::function<Test*()> factory) {
    TestInfo info;
    info.testSuiteName = suiteName;
    info.testName = testName;
    info.factory = factory;
    tests.push_back(info);
}

void UnitTest::ClearFailures() {
    for (auto& t : tests) {
        t.passed = true;
        t.failureMessages.clear();
        t.durationMs = 0.0;
    }
}

int UnitTest::RunAllTests(const std::string& filter) {
    int totalRun = 0;
    int passedCount = 0;
    int failedCount = 0;

    std::cout << "\033[32m[==========]\033[0m Running " << tests.size() << " tests.\n";
    auto totalStart = std::chrono::high_resolution_clock::now();

    for (auto& info : tests) {
        if (!filter.empty()) {
            std::string full = info.testSuiteName + "." + info.testName;
            if (full.find(filter) == std::string::npos) {
                continue;
            }
        }

        totalRun++;
        std::cout << "\033[32m[ RUN      ]\033[0m " << info.testSuiteName << "." << info.testName << "\n";

        s_currentTestFailed = false;
        s_currentTestErrors.clear();

        auto start = std::chrono::high_resolution_clock::now();
        Test* testInstance = info.factory();
        try {
            testInstance->TestBody();
        } catch (const std::exception& ex) {
            CurrentTestFail(std::string("Exception thrown: ") + ex.what());
        } catch (...) {
            CurrentTestFail("Unknown exception thrown");
        }
        delete testInstance;
        auto end = std::chrono::high_resolution_clock::now();

        double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
        info.durationMs = elapsed;

        if (s_currentTestFailed) {
            info.passed = false;
            info.failureMessages = s_currentTestErrors;
            failedCount++;
            for (const auto& err : s_currentTestErrors) {
                std::cout << "\033[31m[  FAILED  ]\033[0m " << err << "\n";
            }
            std::cout << "\033[31m[  FAILED  ]\033[0m " << info.testSuiteName << "." << info.testName << " (" << std::fixed << std::setprecision(1) << elapsed << " ms)\n";
        } else {
            info.passed = true;
            passedCount++;
            std::cout << "\033[32m[       OK ]\033[0m " << info.testSuiteName << "." << info.testName << " (" << std::fixed << std::setprecision(1) << elapsed << " ms)\n";
        }
    }

    auto totalEnd = std::chrono::high_resolution_clock::now();
    double totalElapsed = std::chrono::duration<double, std::milli>(totalEnd - totalStart).count();

    std::cout << "\033[32m[==========]\033[0m " << totalRun << " tests ran. (" << totalElapsed << " ms total)\n";
    std::cout << "\033[32m[  PASSED  ]\033[0m " << passedCount << " tests.\n";
    if (failedCount > 0) {
        std::cout << "\033[31m[  FAILED  ]\033[0m " << failedCount << " tests.\n";
        return 1;
    }
    return 0;
}

void InitGoogleTest(int* argc, char** argv) {
    // Standard GTest init signature
}

void Test::RecordProperty(const std::string&, const std::string&) {}

} // namespace testing
