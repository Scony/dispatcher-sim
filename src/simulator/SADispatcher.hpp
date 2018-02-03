#pragma once

#include "RandomDispatcher.hpp"

class SADispatcher : public RandomDispatcher
{
public:
  SADispatcher(std::shared_ptr<Input> input,
	       std::shared_ptr<Cloud> cloud,
	       std::shared_ptr<IEstimator> estimator,
	       unsigned iterations);

  virtual void dispatch(JobSP job) override;

  void sa();

protected:
  unsigned mIterations;
};
