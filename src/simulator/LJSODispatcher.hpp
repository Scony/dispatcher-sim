#pragma once

#include <map>
#include <vector>

#include "SJLODispatcher.hpp"

class LJSODispatcher : public SJLODispatcher
{
public:
  LJSODispatcher(std::shared_ptr<Input> input,
		 std::shared_ptr<ICloud> cloud,
		 std::shared_ptr<IEstimator> estimator);

  virtual void dispatch(JobSP job) override;
};
