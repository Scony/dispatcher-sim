#pragma once

#include <map>
#include <memory>

#include "Job.hpp"
#include "Operation.hpp"

class Input
{
public:
  Input();

private:
  std::map<long long, std::shared_ptr<Job> > mJobs;
  std::map<long long, std::shared_ptr<Operation> > mOperations;
};
