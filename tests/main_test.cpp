#include "gtest/gtest.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    std::string filter = "";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind("--gtest_filter=", 0) == 0) {
            filter = arg.substr(15);
        }
    }

    return RUN_ALL_TESTS();
}
