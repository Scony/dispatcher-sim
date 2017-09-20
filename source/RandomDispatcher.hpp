#pragma once

#include "SimpleDispatcher.hpp"

class RandomDispatcher : public SimpleDispatcher
{
public:
  RandomDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud);

  void dispatch(std::shared_ptr<Job> job, Queue queue) override;
};
