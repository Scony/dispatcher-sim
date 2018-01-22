#pragma once

#include <vector>
#include <memory>

#include "Operation.hpp"
#include "Job.hpp"
#include "IExecutionsListener.hpp"

class Solution : public IExecutionsListener
{
public:
  using SolutionVec = std::vector<std::pair<long long, OperationSP> >;
  using JobFlowVec = std::vector<std::pair<long long, JobSP> >;

public:
  static long long evalTotalFlow(const SolutionVec& solution);

public:
  void handleNotification(const std::pair<long long, OperationSP>& notification) override;

  void validate(std::vector<JobSP> jobs, unsigned machinesNum);
  JobFlowVec calculateJobFlowVec(std::vector<JobSP> jobs);

private:
  SolutionVec mSolutionVec;
};
