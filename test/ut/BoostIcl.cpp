#include <gtest/gtest.h>

#include <boost/icl/interval_map.hpp>

struct BoostIclTests : testing::Test
{
  boost::icl::interval_map<int, int> sut;

  unsigned continuousIntervalsNum(boost::icl::interval_map<int, int>& intervalTree)
  {
    unsigned counter = 0;
    for (auto it = intervalTree.begin(); it != intervalTree.end(); it++)
      counter++;
    return counter;
  }
};

TEST_F(BoostIclTests, IntervalMapWorksAsExpected)
{
  sut += std::make_pair(boost::icl::discrete_interval<int>::right_open(0,10), 1);
  EXPECT_EQ(sut.size(), 10u);
  EXPECT_EQ(continuousIntervalsNum(sut), 1u);
}

TEST_F(BoostIclTests, IntervalMapWorksAsExpected2)
{
  sut += std::make_pair(boost::icl::discrete_interval<int>::right_open(0,10), 1);
  sut += std::make_pair(boost::icl::discrete_interval<int>::right_open(10,20), 1);
  EXPECT_EQ(sut.size(), 20u);
  EXPECT_EQ(continuousIntervalsNum(sut), 1u);
}

TEST_F(BoostIclTests, IntervalMapWorksAsExpected3)
{
  sut += std::make_pair(boost::icl::discrete_interval<int>::right_open(0,10), 1);
  sut += std::make_pair(boost::icl::discrete_interval<int>::right_open(5,6), 1);
  sut += std::make_pair(boost::icl::discrete_interval<int>::right_open(0,5), 1);
  sut += std::make_pair(boost::icl::discrete_interval<int>::right_open(6,10), 1);
  EXPECT_EQ(sut.size(), 10u);
  EXPECT_EQ(continuousIntervalsNum(sut), 1u);
}

TEST_F(BoostIclTests, IntervalMapWorksAsExpected4)
{
  sut += std::make_pair(boost::icl::discrete_interval<int>::right_open(0,10), 1);
  sut += std::make_pair(boost::icl::discrete_interval<int>::right_open(5,6), 1);
  EXPECT_EQ(sut.size(), 10u);
  EXPECT_EQ(continuousIntervalsNum(sut), 3u);
}
