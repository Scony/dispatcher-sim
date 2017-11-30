#pragma once

#include "MaxDispatcher.hpp"

class MinDispatcher : public MaxDispatcher
{
public:
  MinDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud);

  void dispatch(JobSP job) override;
};
