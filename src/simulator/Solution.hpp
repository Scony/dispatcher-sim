#pragma once

#include <vector>
#include <memory>

#include "Operation.hpp"
#include "Job.hpp"
#include "IExecutionsListener.hpp"

class Solution : public IExecutionsListener
{
public:
  using SolutionVec = std::vector<Assignation>;
  using JobFlowVec = std::vector<std::pair<long long, JobSP> >;

public:
  static long long evalTotalFlow(const SolutionVec& solution);

public:
  void handleNotification(const Assignation& notification) override;

  void validate(std::vector<JobSP> jobs, unsigned machinesNum);
  JobFlowVec calculateJobFlowVec(std::vector<JobSP> jobs);

  SolutionVec getSolutionVec() { return mSolutionVec; }

private:
  SolutionVec mSolutionVec;
};
