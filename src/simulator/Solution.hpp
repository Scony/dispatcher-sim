#pragma once

#include <vector>
#include <memory>

#include "Operation.hpp"
#include "Job.hpp"
#include "IExecutionsListener.hpp"
#include "Machine.hpp"

class Solution : public IExecutionsListener
{
 public:
  using SolutionVec = std::vector<Assignation>;
  using JobFlowVec = std::vector<std::pair<long long, JobSP> >;

 public:
  static long long evalTotalFlow(const SolutionVec& solution);

 public:
  static bool validateOperationEnds(const SolutionVec& solution);
  static bool validateSingularOperationExecutions(const SolutionVec& solution,
						  const std::vector<JobSP>& jobs);
  static bool validateMachineCapacityUsage(const SolutionVec& solution,
					   const std::vector<MachineSP>& machines);

 public:
  void handleNotification(const Assignation& notification) override;

  JobFlowVec calculateJobFlowVec(std::vector<JobSP> jobs);

  SolutionVec getSolutionVec() { return mSolutionVec; }

 private:
  SolutionVec mSolutionVec;
};
