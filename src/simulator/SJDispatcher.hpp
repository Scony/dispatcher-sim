#pragma once

#include <map>
#include <vector>

#include "Dispatcher.hpp"

class SJDispatcher : public Dispatcher
{
 public:
  using Dispatcher::Dispatcher;

  OperationSP peek() override;
  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(JobSP job) override;

 protected:
  virtual void reorderJobOperations(std::vector<OperationSP>&);
  void updateJobWeights();

  std::map<long long, std::vector<OperationSP>> mJobOperations;
  std::vector<std::pair<long long, long long>> mJobsInOrder;
};
