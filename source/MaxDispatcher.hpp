#pragma once

#include "IDispatcher.hpp"

class MaxDispatcher : public IDispatcher
{
public:
  MaxDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud);

  void dispatch(JobSP job) override;
};
