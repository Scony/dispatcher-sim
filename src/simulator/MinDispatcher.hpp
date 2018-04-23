#pragma once

#include "MaxDispatcher.hpp"

class MinDispatcher : public MaxDispatcher
{
 public:
  MinDispatcher(std::shared_ptr<Input> input,
		std::shared_ptr<ICloud> cloud,
		std::shared_ptr<IEstimator> estimator);

  void dispatch(JobSP job) override;
};
