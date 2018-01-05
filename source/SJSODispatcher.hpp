#pragma once

#include <map>
#include <vector>

#include "SJLODispatcher.hpp"

class SJSODispatcher : public SJLODispatcher
{
public:
  SJSODispatcher(std::shared_ptr<Input> input,
  		 std::shared_ptr<Cloud> cloud,
  		 std::shared_ptr<IEstimator> estimator);

  virtual void dispatch(JobSP job) override;
};
