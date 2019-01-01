#pragma once

#include "SASADispatcher.hpp"

class SJSADispatcher : public SASADispatcher
{
 public:
  SJSADispatcher(
      std::shared_ptr<Input> input,
      std::shared_ptr<ICloud> cloud,
      std::shared_ptr<IEstimator> estimator,
      std::string operationLevelAlgorithm,
      unsigned iterations);

  virtual void dispatch(std::shared_ptr<Job> job) override;
};
