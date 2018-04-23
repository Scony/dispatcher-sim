#pragma once

#include <map>

#include "Dispatcher.hpp"

class MaxDispatcher : public Dispatcher
{
 public:
  MaxDispatcher(std::shared_ptr<Input> input,
		std::shared_ptr<ICloud> cloud,
		std::shared_ptr<IEstimator> estimator);

  OperationSP peek() override;
  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(JobSP job) override;

 protected:
  std::map<long long, std::vector<OperationSP> > mJobOperations;
  unsigned mNextJob;
};
