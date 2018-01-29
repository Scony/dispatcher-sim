#pragma once

#include <memory>

#include "Input.hpp"
#include "Cloud.hpp"
#include "IQueue.hpp"
#include "IEstimator.hpp"

class Dispatcher : public IQueue
{
public:
  Dispatcher(std::shared_ptr<Input> input,
	     std::shared_ptr<Cloud> cloud,
	     std::shared_ptr<IEstimator> estimator);

  virtual void dispatch(std::shared_ptr<Job> job) = 0;
  virtual void advance(long long toTimestamp);

protected:
  std::shared_ptr<Input> mInput;
  std::shared_ptr<Cloud> mCloud;
  std::shared_ptr<IEstimator> mEstimator;
};