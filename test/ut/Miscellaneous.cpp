#include <algorithm>

#include <gtest/gtest.h>

#include "Fuzzy3.hpp"

TEST(MiscellaneousTests, TestTupleSorting)
{
  std::vector<std::tuple<int, int, int> > vec = {
    {3,3,3},
    {2,3,3},
    {1,3,3},
    {3,2,3},
    {3,1,3},
    {3,2,2},
    {3,2,1},
  };
  std::vector<std::tuple<int, int, int> > expectedVec = {
    {1,3,3},
    {2,3,3},
    {3,1,3},
    {3,2,1},
    {3,2,2},
    {3,2,3},
    {3,3,3},
  };

  std::sort(vec.begin(), vec.end());

  EXPECT_EQ(vec, expectedVec);
}
