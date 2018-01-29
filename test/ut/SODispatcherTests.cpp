#include <gtest/gtest.h>

#include "SODispatcher.hpp"

TEST(SODispatcherTests, OrderCheck) {
  auto op1 = std::make_shared<Operation>(1,1,1,1,10,30);
  auto op2 = std::make_shared<Operation>(2,1,1,1,10,35);
  auto op3 = std::make_shared<Operation>(3,1,1,1,5,30);
  std::vector<OperationSP> operations = {op1, op2, op3};
  auto job = std::make_shared<Job>(1,1,1,operations);

  auto cloud = std::make_shared<Cloud>(1337);
  SODispatcher dispatcher(std::shared_ptr<Input>(nullptr),
			  cloud,
			  std::shared_ptr<IEstimator>(nullptr));

  dispatcher.dispatch(job);

  auto pop1 = dispatcher.pop();
  EXPECT_EQ(pop1->id, 3);

  auto pop2 = dispatcher.pop();
  EXPECT_EQ(pop2->id, 1);

  auto pop3 = dispatcher.pop();
  EXPECT_EQ(pop3->id, 2);
}
