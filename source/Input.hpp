#pragma once

#include <vector>
// #include <map>
#include <memory>

#include "Job.hpp"
#include "Operation.hpp"

class Input
{
public:
  Input();

  void readFromStdin();

private:
  std::vector<std::shared_ptr<Job> > mQueue;
  // std::map<long long, std::shared_ptr<Job> > mJobs;
  // std::map<long long, std::shared_ptr<Operation> > mOperations;
};
