#include <gtest/gtest.h>

#include "Fuzzy3.hpp"

TEST(Fuzzy3Tests, Comparison1)
{
  auto a = Fuzzy3(0, 2, 4);
  auto b = Fuzzy3(5, 7, 9);
  EXPECT_EQ(a < b, true);
}

TEST(Fuzzy3Tests, Comparison2)
{
  auto a = Fuzzy3(0, 2, 2);
  auto b = Fuzzy3(1, 2, 2);
  EXPECT_EQ(a < b, true);
}

TEST(Fuzzy3Tests, Comparison3)
{
  auto a = Fuzzy3(0, 0, 1);
  auto b = Fuzzy3(0, 0, 2);
  EXPECT_EQ(a < b, true);
}

TEST(Fuzzy3Tests, Comparison4)
{
  auto a = Fuzzy3(1, 2, 2);
  auto b = Fuzzy3(0, 2, 10);
  EXPECT_EQ(a < b, true);
}

TEST(Fuzzy3Tests, Comparison5)
{
  auto a = Fuzzy3(1, 2, 3);
  auto b = Fuzzy3(0, 2, 10);
  EXPECT_EQ(a < b, true);
}
