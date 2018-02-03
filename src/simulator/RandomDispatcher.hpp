#pragma once

#include "Dispatcher.hpp"

class RandomDispatcher : public Dispatcher
{
public:
  RandomDispatcher(std::shared_ptr<Input> input,
		   std::shared_ptr<Cloud> cloud,
		   std::shared_ptr<IEstimator> estimator);

  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(JobSP job) override;

protected:
  std::vector<OperationSP> mQueue;
};
