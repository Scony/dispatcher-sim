#pragma once

#include <map>
#include <vector>

#include "Dispatcher.hpp"

class SJLODispatcher : public Dispatcher
{
 public:
  SJLODispatcher(std::shared_ptr<Input> input,
		 std::shared_ptr<ICloud> cloud,
		 std::shared_ptr<IEstimator> estimator);

  OperationSP peek() override;
  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(std::vector<JobSP>) override;

 protected:
  std::map<long long, std::vector<OperationSP> > mJobOperations;
  std::vector<std::pair<long long, long long> > mJobsInOrder;
};
