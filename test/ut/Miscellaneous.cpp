#include <algorithm>
#include <queue>

#include <gtest/gtest.h>
// #include <boost/heap/fibonacci_heap.hpp>

#include "Fuzzy3.hpp"

TEST(MiscellaneousTests, TestTupleSorting) {
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

TEST(MiscellaneousTests, TestPriorityQueue)
{
  using Element = std::tuple<long long, long long>;
  std::priority_queue<Element, std::vector<Element>, std::greater<Element> > heap;
  heap.emplace(1,1);
  heap.emplace(1,2);
  heap.emplace(2,1);
  heap.emplace(2,2);
  EXPECT_EQ(heap.top(), std::make_tuple(1,1));
  heap.pop();
  EXPECT_EQ(heap.top(), std::make_tuple(1,2));
  heap.pop();
  EXPECT_EQ(heap.top(), std::make_tuple(2,1));
  heap.pop();
  EXPECT_EQ(heap.top(), std::make_tuple(2,2));
}

// TEST(MiscellaneousTests, TestFibonacciHeap)
// {
//   using Element = std::tuple<long long, long long>;
//   boost::heap::fibonacci_heap<Element, boost::heap::compare<std::greater<Element> > > heap;
//   heap.emplace(1,1);
//   heap.emplace(1,2);
//   heap.emplace(2,1);
//   heap.emplace(2,2);
//   EXPECT_EQ(heap.top(), std::make_tuple(1,1));
//   heap.pop();
//   EXPECT_EQ(heap.top(), std::make_tuple(1,2));
//   heap.pop();
//   EXPECT_EQ(heap.top(), std::make_tuple(2,1));
//   heap.pop();
//   EXPECT_EQ(heap.top(), std::make_tuple(2,2));
// }
