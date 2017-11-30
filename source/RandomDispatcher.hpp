#pragma once

#include "IDispatcher.hpp"

class RandomDispatcher : public IDispatcher
{
public:
  RandomDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud);

  void dispatch(JobSP job) override;
};
