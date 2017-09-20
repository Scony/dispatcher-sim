#pragma once

#include "SimpleDispatcher.hpp"

class MaxDispatcher : public SimpleDispatcher
{
public:
  MaxDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud);

  void dispatch(JobSP job, Queue queue) override;
};
