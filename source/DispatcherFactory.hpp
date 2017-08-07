#pragma once

#include <vector>
#include <string>

#include "IDispatcher.hpp"

class DispatcherFactory
{
public:
  DispatcherFactory(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud, std::vector<std::string> args) :
    mInput(input),
    mCloud(cloud),
    mArgs(args) {}

  std::shared_ptr<IDispatcher> getDispatcher() { return std::shared_ptr<IDispatcher>(nullptr); }

private:
  std::shared_ptr<Input> mInput;
  std::shared_ptr<Cloud> mCloud;
  std::vector<std::string> mArgs;
};
