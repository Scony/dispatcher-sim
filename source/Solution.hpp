#pragma once

#include <vector>
#include <memory>

#include "Operation.hpp"
#include "Job.hpp"
#include "IExecutionsListener.hpp"

class Solution : public IExecutionsListener
{
public:
  using SolutionVec = std::vector<std::pair<long long, std::shared_ptr<Operation> > >;
  using JobFlowVec = std::vector<std::pair<long long, std::shared_ptr<Job> > >;

public:
  void handleNotification(const std::pair<long long, OperationSP>& notification) override;

  void validate(std::vector<std::shared_ptr<Job> > jobs, unsigned machinesNum);
  JobFlowVec calculateJobFlowVec(std::vector<std::shared_ptr<Job> > jobs);

private:
  SolutionVec mSolutionVec;
};
