#pragma once

#include <map>
#include <vector>

#include "Dispatcher.hpp"

class SJLODispatcher : public Dispatcher
{
 public:
  using Dispatcher::Dispatcher;

  OperationSP peek() override;
  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(JobSP job) override;

 protected:
  std::map<long long, std::vector<OperationSP> > mJobOperations;
  std::vector<std::pair<long long, long long> > mJobsInOrder;
};
