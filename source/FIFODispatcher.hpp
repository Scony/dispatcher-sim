#pragma once

#include <deque>

#include "Dispatcher.hpp"

class FIFODispatcher : public Dispatcher
{
public:
  FIFODispatcher(std::shared_ptr<Input> input,
		 std::shared_ptr<Cloud> cloud,
		 std::shared_ptr<IEstimator> estimator);

  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(JobSP job) override;

protected:
  std::deque<OperationSP> mOperations;
};
