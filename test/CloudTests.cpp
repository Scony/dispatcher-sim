#include <gtest/gtest.h>

#include "Cloud.hpp"

TEST(CloudTests, Process1) {
  auto op1 = std::make_shared<Operation>(1,1,1,1,5,5);
  auto op2 = std::make_shared<Operation>(2,1,1,1,6,5);
  auto op3 = std::make_shared<Operation>(3,1,1,1,7,5);
  std::vector<OperationSP> operations = {op3, op2, op1};

  auto schedule = Cloud::process(1,operations);
  ASSERT_EQ(schedule.size(), 3);
  EXPECT_EQ(schedule[0].first, 10);
  EXPECT_EQ(schedule[1].first, 15);
  EXPECT_EQ(schedule[2].first, 20);
}

TEST(CloudTests, Process2) {
  auto op1 = std::make_shared<Operation>(1,1,1,1,5,5);
  auto op2 = std::make_shared<Operation>(2,1,1,1,10,5);
  auto op3 = std::make_shared<Operation>(3,1,1,1,15,5);
  std::vector<OperationSP> operations = {op3, op2, op1};

  auto schedule = Cloud::process(1,operations);
  ASSERT_EQ(schedule.size(), 3);
  EXPECT_EQ(schedule[0].first, 10);
  EXPECT_EQ(schedule[1].first, 15);
  EXPECT_EQ(schedule[2].first, 20);
}

TEST(CloudTests, Process3) {
  auto op1 = std::make_shared<Operation>(1,1,1,1,5,5);
  auto op2 = std::make_shared<Operation>(2,1,1,1,11,5);
  auto op3 = std::make_shared<Operation>(3,1,1,1,4,5);
  std::vector<OperationSP> operations = {op3, op2, op1};

  auto schedule = Cloud::process(1,operations);
  ASSERT_EQ(schedule.size(), 3);
  EXPECT_EQ(schedule[0].first, 10);
  EXPECT_EQ(schedule[1].first, 16);
  EXPECT_EQ(schedule[2].first, 21);
}
