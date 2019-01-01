#pragma once

#include "RandomDispatcher.hpp"

class SADispatcher : public RandomDispatcher
{
 public:
  SADispatcher(
      std::shared_ptr<Input> input,
      std::shared_ptr<ICloud> cloud,
      std::shared_ptr<IEstimator> estimator,
      unsigned iterations);

  virtual void dispatch(JobSP job) override;

 protected:
  unsigned mIterations;
};
