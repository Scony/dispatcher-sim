#pragma once

#include <map>

#include "IDispatcher.hpp"

class MaxDispatcher : public IDispatcher
{
public:
  MaxDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud);

  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(JobSP job) override;

protected:
  std::map<long long, std::vector<OperationSP> > mJobOperations;
  unsigned mNextJob;
};
