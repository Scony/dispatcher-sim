#include <gtest/gtest.h>

#include "Cloud.hpp"
#include "Solution.hpp"

// TEST(SolutionTests, EvalTotalFlow1) {
//   auto op1 = std::make_shared<Operation>(1,1,1,1,5,5);
//   auto op2 = std::make_shared<Operation>(2,1,1,1,5,5);
//   auto op3 = std::make_shared<Operation>(3,1,1,1,5,5);
//   std::vector<OperationSP> operations = {op3, op2, op1};

//   auto schedule = Cloud::simulate(1,operations);
//   ASSERT_EQ(schedule.size(), 3);
//   EXPECT_EQ(Solution::evalTotalFlow(schedule), 15);
// }

// TEST(SolutionTests, EvalTotalFlow2) {
//   auto op1 = std::make_shared<Operation>(1,1,1,1,5,5);
//   auto op2 = std::make_shared<Operation>(2,1,1,1,5,5);
//   auto op3 = std::make_shared<Operation>(3,1,1,1,5,5);
//   std::vector<OperationSP> operations = {op3, op2, op1};

//   auto schedule = Cloud::simulate(3,operations);
//   ASSERT_EQ(schedule.size(), 3);
//   EXPECT_EQ(Solution::evalTotalFlow(schedule), 5);
// }
