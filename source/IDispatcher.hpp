#pragma once

#include <memory>

#include "Input.hpp"
#include "Cloud.hpp"

class IDispatcher
{
public:
  IDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud) :
    mInput(input),
    mCloud(cloud) {}

  virtual void dispatch(std::shared_ptr<Job> job) = 0;
  virtual void advance(long long toTimestamp);

protected:
  std::shared_ptr<Input> mInput;
  std::shared_ptr<Cloud> mCloud;
};
