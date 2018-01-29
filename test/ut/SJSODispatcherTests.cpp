#include <gtest/gtest.h>

#include "SJSODispatcher.hpp"
#include "NoEstimator.hpp"

TEST(SJSODispatcherTests, OrderCheck) {
  auto job0 = std::make_shared<Job>(0,1,1,std::vector<OperationSP>{
      std::make_shared<Operation>(0,0,1,0,1,1),
	std::make_shared<Operation>(1,0,1,0,1,99),
	});

  auto job1 = std::make_shared<Job>(1,1,2,std::vector<OperationSP>{
      std::make_shared<Operation>(2,1,1,0,2,1),
	});

  auto job2 = std::make_shared<Job>(2,1,3,std::vector<OperationSP>{
      std::make_shared<Operation>(3,2,1,0,3,1),
	});

  auto cloud = std::make_shared<Cloud>(2);
  SJSODispatcher dispatcher(std::shared_ptr<Input>(nullptr),
			    cloud,
			    std::shared_ptr<IEstimator>(new NoEstimator));

  dispatcher.dispatch(job0);

  auto pop1 = dispatcher.pop();
  EXPECT_EQ(pop1->id, 0);

  dispatcher.dispatch(job1);

  auto pop2 = dispatcher.pop();
  EXPECT_EQ(pop2->id, 2);

  dispatcher.dispatch(job2);

  auto pop3 = dispatcher.pop();
  EXPECT_EQ(pop3->id, 3);

  auto pop4 = dispatcher.pop();
  EXPECT_EQ(pop4->id, 1);
}
