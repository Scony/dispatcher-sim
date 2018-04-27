#include <algorithm>
#include <queue>

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

TEST(MiscellaneousTests, TestVectorIterators)
{
  std::vector<int> vec = { 1, 2, 3 };
  EXPECT_EQ(*vec.begin(), 1);
  EXPECT_EQ(*(vec.begin()+1), 2);
  EXPECT_EQ(*(vec.begin()+2), 3);
  EXPECT_EQ(vec.begin()+3, vec.end());
  EXPECT_NE(vec.begin()+99, vec.end());
  vec.insert(vec.begin()+3, 4);
  EXPECT_EQ(vec[3], 4);

  std::vector<int> emptyVec = { };
  EXPECT_EQ(emptyVec.begin(), emptyVec.end());
  EXPECT_EQ(emptyVec.begin()+0, emptyVec.end());
}

TEST(MiscellaneousTests, TestPriorityQueueOrdering)
{
  using Pair = std::pair<int, int>;
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair> > pq;

  pq.push({1,2});
  pq.push({2,1});
  pq.push({2,2});
  EXPECT_EQ(pq.top(), Pair(1, 2));
  pq.pop();
  EXPECT_EQ(pq.top(), Pair(2, 1));
  pq.pop();
  EXPECT_EQ(pq.top(), Pair(2, 2));
}
