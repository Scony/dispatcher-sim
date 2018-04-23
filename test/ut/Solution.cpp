#include <gtest/gtest.h>

#include "Cloud.hpp"
#include "Solution.hpp"

TEST(SolutionTests, ValidateOperationEnds1)
{
  auto op = std::make_shared<Operation>(1,1,1,1,0,5);
  std::vector<Assignation> assignations = { std::make_tuple(999, op, 1) };
  EXPECT_EQ(Solution::validateOperationEnds(assignations), true);
}

TEST(SolutionTests, ValidateOperationEnds2)
{
  auto op = std::make_shared<Operation>(1,1,1,1,0,5);
  std::vector<Assignation> assignations = { std::make_tuple(5, op, 1) };
  EXPECT_EQ(Solution::validateOperationEnds(assignations), true);
}

TEST(SolutionTests, ValidateOperationEnds3)
{
  auto op = std::make_shared<Operation>(1,1,1,1,0,5);
  std::vector<Assignation> assignations = { std::make_tuple(4, op, 1) };
  EXPECT_EQ(Solution::validateOperationEnds(assignations), false);
}

TEST(SolutionTests, ValidateOperationEnds4)
{
  auto op = std::make_shared<Operation>(1,1,1,1,5,5);
  std::vector<Assignation> assignations = { std::make_tuple(4, op, 1) };
  EXPECT_EQ(Solution::validateOperationEnds(assignations), false);
}

TEST(SolutionTests, ValidateSingularOperationExecutions1)
{
  auto op = std::make_shared<Operation>(1,1,1,1,0,5);
  std::vector<OperationSP> operations = { op };
  auto job = std::make_shared<Job>(1, 1, 0, operations);
  std::vector<Assignation> assignations = { std::make_tuple(0, op, 1) };
  std::vector<JobSP> jobs = { job };
  EXPECT_EQ(Solution::validateSingularOperationExecutions(assignations, jobs), true);
}

TEST(SolutionTests, ValidateSingularOperationExecutions2)
{
  auto op = std::make_shared<Operation>(1,1,1,1,0,5);
  std::vector<OperationSP> operations = { op };
  auto job = std::make_shared<Job>(1, 1, 0, operations);
  std::vector<Assignation> assignations = { };
  std::vector<JobSP> jobs = { job };
  EXPECT_EQ(Solution::validateSingularOperationExecutions(assignations, jobs), false);
}

TEST(SolutionTests, ValidateSingularOperationExecutions3)
{
  auto op = std::make_shared<Operation>(1,1,1,1,0,5);
  std::vector<OperationSP> operations = { op };
  auto job = std::make_shared<Job>(1, 1, 0, operations);
  std::vector<Assignation> assignations = { std::make_tuple(0, op, 1), std::make_tuple(10, op, 1) };
  std::vector<JobSP> jobs = { job };
  EXPECT_EQ(Solution::validateSingularOperationExecutions(assignations, jobs), false);
}

TEST(SolutionTests, ValidateMachineCapacityUsageWell)
{
  auto op1 = std::make_shared<Operation>(1,1,1,1,0,5,1);
  std::vector<Assignation> assignations = { std::make_tuple(10, op1, 1) };
  std::vector<MachineSP> machines = { std::make_shared<Machine>(1,1) };
  EXPECT_EQ(Solution::validateMachineCapacityUsage(assignations, machines), true);
}

TEST(SolutionTests, ValidateMachineCapacityUsageNonExisting)
{
  auto op1 = std::make_shared<Operation>(1,1,1,1,0,5,1);
  std::vector<Assignation> assignations = { std::make_tuple(10, op1, 1) };
  std::vector<MachineSP> machines = { };
  EXPECT_EQ(Solution::validateMachineCapacityUsage(assignations, machines), false);
}

TEST(SolutionTests, ValidateMachineCapacityUsageNotEnoguh)
{
  auto op1 = std::make_shared<Operation>(1,1,1,1,0,5,2);
  std::vector<Assignation> assignations = { std::make_tuple(10, op1, 1) };
  std::vector<MachineSP> machines = { std::make_shared<Machine>(1,1) };
  EXPECT_EQ(Solution::validateMachineCapacityUsage(assignations, machines), false);
}

TEST(SolutionTests, ValidateMachineCapacityUsageNotExhausted)
{
  auto op1 = std::make_shared<Operation>(1,1,1,1,0,5,1);
  auto op2 = std::make_shared<Operation>(1,1,1,1,0,5,1);
  std::vector<Assignation> assignations = { std::make_tuple(10, op1, 1), std::make_tuple(10, op2, 1) };
  std::vector<MachineSP> machines = { std::make_shared<Machine>(1,2) };
  EXPECT_EQ(Solution::validateMachineCapacityUsage(assignations, machines), true);
}

TEST(SolutionTests, ValidateMachineCapacityUsageExhausted)
{
  auto op1 = std::make_shared<Operation>(1,1,1,1,0,5,1);
  auto op2 = std::make_shared<Operation>(1,1,1,1,0,5,1);
  std::vector<Assignation> assignations = { std::make_tuple(10, op1, 1), std::make_tuple(10, op2, 1) };
  std::vector<MachineSP> machines = { std::make_shared<Machine>(1,1) };
  EXPECT_EQ(Solution::validateMachineCapacityUsage(assignations, machines), false);
}
