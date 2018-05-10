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
  using JobStretchVec = std::vector<std::pair<double, JobSP> >;

 public:
  static long long evalTotalFlow(const SolutionVec& solution);
  static JobFlowVec calculateJobFlowVec(const SolutionVec& solution, std::vector<JobSP> jobs);
  static JobStretchVec calculateJobStretchVec(const SolutionVec& solution, std::vector<JobSP> jobs);
  static bool validateOperationEnds(const SolutionVec& solution);
  static bool validateSingularOperationExecutions(const SolutionVec& solution,
						  const std::vector<JobSP>& jobs);
  static bool validateMachineCapacityUsage(const SolutionVec& solution,
					   const std::vector<MachineSP>& machines);

 public:
  void handleNotification(const Assignation& notification) override;

  SolutionVec getSolutionVec() { return mSolutionVec; }

 private:
  SolutionVec mSolutionVec;
};
