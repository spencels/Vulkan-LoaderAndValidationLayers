#include <vector>
#include <string>
#include <gtest/gtest.h>

#include "check_error_codes.h"


TEST(SplitString, EmptyPart) {
  auto parts = SplitString("1,,,,2", ",,");
  EXPECT_EQ(3, parts.size());
  EXPECT_EQ("1", parts[0]);
  EXPECT_EQ("", parts[1]);
  EXPECT_EQ("2", parts[2]);
}


TEST(SplitString, MultiPartTest) {
  auto parts = SplitString("1,,2,,3", ",,");
  EXPECT_EQ(3, parts.size());
  EXPECT_EQ("1", parts[0]);
  EXPECT_EQ("2", parts[1]);
  EXPECT_EQ("3", parts[2]);
}