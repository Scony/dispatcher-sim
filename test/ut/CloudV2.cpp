#include <gtest/gtest.h>

#include "CloudV2.hpp"
#include "NoEstimator.hpp"
#include "VectorQueue.hpp"

TEST(CloudV2, BusyMachineCmp1) {
  auto bm1 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 5, 0);
  auto bm2 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 6, 0);
  EXPECT_LT(bm1, bm2);
  EXPECT_FALSE(bm2 < bm1);
}

TEST(CloudV2, BusyMachineCmp2) {
  auto bm1 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 5, 0);
  auto bm2 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 5, 1);
  EXPECT_LT(bm1, bm2);
  EXPECT_FALSE(bm2 < bm1);
}

TEST(CloudV2, BusyMachineCmp3) {
  auto bm1 = CloudV2::BusyMachine(MachineSP(nullptr), std::make_shared<Operation>(1,0,0,0,0,0,0), 5, 0);
  auto bm2 = CloudV2::BusyMachine(MachineSP(nullptr), std::make_shared<Operation>(2,0,0,0,0,0,0), 5, 0);
  EXPECT_LT(bm1, bm2);
  EXPECT_FALSE(bm2 < bm1);
}

TEST(CloudV2, BusyMachines1) {
  auto bm1 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 5, 0);
  auto bm2 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 6, 0);
  CloudV2::BusyMachines busyMachines;
  busyMachines.push(bm1);
  busyMachines.push(bm2);
  EXPECT_EQ(busyMachines.top(), bm1);
  busyMachines.pop();
  EXPECT_EQ(busyMachines.top(), bm2);
}

TEST(CloudV2, BusyMachines2) {
  auto bm1 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 5, 0);
  auto bm2 = CloudV2::BusyMachine(MachineSP(nullptr), OperationSP(nullptr), 6, 0);
  CloudV2::BusyMachines busyMachines;
  busyMachines.push(bm2);
  busyMachines.push(bm1);
  EXPECT_EQ(busyMachines.top(), bm1);
  busyMachines.pop();
  EXPECT_EQ(busyMachines.top(), bm2);
}

TEST(CloudV2, EmptyQueueNoMachines) {
  long long fromTimestamp = 0;
  long long toTimestamp = 9999;
  IEstimatorSP noEstimator(new NoEstimator);
  std::vector<OperationSP> operations = {};
  std::shared_ptr<IQueue> queue = std::make_shared<VectorQueue>(operations);
  CloudV2::FreeMachines freeMachines = {};
  CloudV2::BusyMachines busyMachines = {};
  long long assignationsCounter = 0;
  unsigned setupTime = 0;

  auto solution = CloudV2::process(fromTimestamp,
				   toTimestamp,
				   noEstimator,
				   queue.get(),
				   freeMachines,
				   busyMachines,
				   assignationsCounter,
				   setupTime);

  EXPECT_EQ(assignationsCounter, 0);
  EXPECT_EQ(solution, std::vector<Assignation>{});
}

TEST(CloudV2, EmptyQueueOneMachine) {
  long long fromTimestamp = 0;
  long long toTimestamp = 9999;
  IEstimatorSP noEstimator(new NoEstimator);
  std::vector<OperationSP> operations = {};
  std::shared_ptr<IQueue> queue = std::make_shared<VectorQueue>(operations);
  CloudV2::FreeMachines freeMachines = {
    {1, {CloudV2::FreeMachine(std::make_shared<Machine>(0,1))}}
  };
  CloudV2::BusyMachines busyMachines = {};
  long long assignationsCounter = 0;
  unsigned setupTime = 0;

  auto solution = CloudV2::process(fromTimestamp,
				   toTimestamp,
				   noEstimator,
				   queue.get(),
				   freeMachines,
				   busyMachines,
				   assignationsCounter,
				   setupTime);

  EXPECT_EQ(assignationsCounter, 0);
  EXPECT_EQ(solution, std::vector<Assignation>{});
}

TEST(CloudV2, SimpleQueueOneMachine1) {
  long long fromTimestamp = 0;
  long long toTimestamp = 9999;
  IEstimatorSP noEstimator(new NoEstimator);
  auto op0 = std::make_shared<Operation>(0,0,0,0,0,1,1);
  auto op1 = std::make_shared<Operation>(1,0,0,0,0,1,1);
  std::vector<OperationSP> operations = {
    op1,
    op0
  };
  std::shared_ptr<IQueue> queue = std::make_shared<VectorQueue>(operations);
  CloudV2::FreeMachines freeMachines = {
    {1, {CloudV2::FreeMachine(std::make_shared<Machine>(0,1))}}
  };
  CloudV2::BusyMachines busyMachines = {};
  long long assignationsCounter = 0;
  unsigned setupTime = 0;

  auto solution = CloudV2::process(fromTimestamp,
				   toTimestamp,
				   noEstimator,
				   queue.get(),
				   freeMachines,
				   busyMachines,
				   assignationsCounter,
				   setupTime);

  EXPECT_EQ(assignationsCounter, 2);

  std::vector<Assignation> expectedSolution = {
    std::make_tuple(1,op0,0),
    std::make_tuple(2,op1,0),
  };
  EXPECT_EQ(solution, expectedSolution);
}

TEST(CloudV2, SimpleQueueOneMachine2) {
  long long fromTimestamp = 0;
  long long toTimestamp = 9999;
  IEstimatorSP noEstimator(new NoEstimator);
  auto op0 = std::make_shared<Operation>(0,0,0,0,0,1,4);
  auto op1 = std::make_shared<Operation>(1,0,0,0,0,1,4);
  std::vector<OperationSP> operations = {
    op1,
    op0
  };
  std::shared_ptr<IQueue> queue = std::make_shared<VectorQueue>(operations);
  CloudV2::FreeMachines freeMachines = {
    {4, {CloudV2::FreeMachine(std::make_shared<Machine>(0,4))}}
  };
  CloudV2::BusyMachines busyMachines = {};
  long long assignationsCounter = 0;
  unsigned setupTime = 0;

  auto solution = CloudV2::process(fromTimestamp,
				   toTimestamp,
				   noEstimator,
				   queue.get(),
				   freeMachines,
				   busyMachines,
				   assignationsCounter,
				   setupTime);

  EXPECT_EQ(assignationsCounter, 2);

  std::vector<Assignation> expectedSolution = {
    std::make_tuple(1,op0,0),
    std::make_tuple(2,op1,0),
  };
  EXPECT_EQ(solution, expectedSolution);
}
