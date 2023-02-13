#include "gtest/gtest.h"
#include "etl/string.h"

using namespace Project::etl;

TEST(String, Append) {
    String<6> s = "Test";
    EXPECT_EQ(s.rem(), 1);
    s += '\n';
    s += "1"; // cannot
    EXPECT_EQ(s, "Test\n");
}

TEST(String, Assign) {
    const String s = "Test";
    EXPECT_EQ(s, "Test");
}

TEST(String, IsContaining) {
    String f;
    f("Test %d%d%d", 1, 2, 3);
    EXPECT_TRUE(f.isContaining("123"));
    EXPECT_TRUE(!f.isContaining("321"));
}