#include <gtest/gtest.h>

#include "SAScheduler.hpp"
#include "NoEstimator.hpp"

TEST(SASchedulerTests, CheckCompleteness)
{
  auto machines = std::make_shared<Machines>(3, 1);
  Schedule schedule(machines);
  auto op1 = std::make_shared<Operation>(1,1,1,1,10,30);
  auto op2 = std::make_shared<Operation>(2,1,1,1,10,35);
  auto op3 = std::make_shared<Operation>(3,1,1,1,5,30);
  std::vector<OperationSP> operations = {op1, op2, op3};
  auto job = std::make_shared<Job>(1,1,1,operations);
  std::vector<JobSP> jobs = { job };

  SAScheduler scheduler(std::make_shared<Input>(jobs),
                        std::shared_ptr<Machines>(nullptr),
                        std::make_shared<NoEstimator>(),
                        100);
  scheduler.schedule(schedule, job);

  std::set<OperationSP> scheduledOperations;
  for (const auto& machineVec : schedule.schedule)
    for (const auto& operation : machineVec)
      scheduledOperations.insert(operation);

  EXPECT_NE(scheduledOperations.find(op1), scheduledOperations.end());
  EXPECT_NE(scheduledOperations.find(op2), scheduledOperations.end());
  EXPECT_NE(scheduledOperations.find(op3), scheduledOperations.end());
}
