#pragma once

#include "Dispatcher.hpp"

class RandomDispatcher : public Dispatcher
{
 public:
  RandomDispatcher(std::shared_ptr<Input> input,
		   std::shared_ptr<ICloud> cloud,
		   std::shared_ptr<IEstimator> estimator);

  OperationSP peek() override;
  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(std::vector<JobSP>) override;

 protected:
  std::vector<OperationSP> mQueue;
};
