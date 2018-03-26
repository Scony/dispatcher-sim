#pragma once

#include <vector>
#include <memory>

#include "Job.hpp"
#include "Operation.hpp"

class Input
{
public:
  Input();

  virtual void readFromStdin();

  std::vector<std::shared_ptr<Job> > getJobs();
  long long getJobsNum();
  long long getOperationsNum();

protected:
  std::vector<std::shared_ptr<Job> > mJobs;
};
