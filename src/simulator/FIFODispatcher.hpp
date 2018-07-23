#pragma once

#include <deque>

#include "Dispatcher.hpp"

class FIFODispatcher : public Dispatcher
{
 public:
  FIFODispatcher(std::shared_ptr<Input> input,
		 std::shared_ptr<ICloud> cloud,
		 std::shared_ptr<IEstimator> estimator);

  OperationSP peek() override;
  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(std::vector<JobSP>) override;

 protected:
  std::deque<OperationSP> mOperations;
};
