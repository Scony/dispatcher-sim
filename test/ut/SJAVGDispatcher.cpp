#include <gtest/gtest.h>

#include "SJAVGDispatcher.hpp"
#include "Cloud.hpp"
#include "NoEstimator.hpp"

TEST(SJAVGDispatcherTests, OrderCheck)
{
  auto op1 = std::make_shared<Operation>(1,1,1,1,0,1);
  auto op2 = std::make_shared<Operation>(2,1,1,1,0,2);
  auto op3 = std::make_shared<Operation>(3,1,1,1,0,20);
  auto op4 = std::make_shared<Operation>(4,1,1,1,0,22);
  std::vector<OperationSP> operations = {op1, op2, op3, op4};
  auto job = std::make_shared<Job>(1,1,0,operations);
  auto cloud = std::make_shared<Cloud>(1337);
  SJAVGDispatcher dispatcher(std::shared_ptr<Input>(nullptr),
                             cloud,
                             std::make_shared<NoEstimator>());

  dispatcher.dispatch(job);

  EXPECT_EQ(dispatcher.pop()->id, 3);
  EXPECT_EQ(dispatcher.pop()->id, 2);
  EXPECT_EQ(dispatcher.pop()->id, 1);
  EXPECT_EQ(dispatcher.pop()->id, 4);
}
