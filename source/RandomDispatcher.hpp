#pragma once

#include "SimpleDispatcher.hpp"

class RandomDispatcher : public SimpleDispatcher
{
public:
  RandomDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud);

  void dispatch(JobSP job, QueueSP queue) override;
};
