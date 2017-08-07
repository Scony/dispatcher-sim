#pragma once

#include <vector>
#include <string>

#include "IDispatcher.hpp"

class DispatcherFactory
{
public:
  DispatcherFactory(std::vector<std::string> args) {}

  std::shared_ptr<IDispatcher> getDispatcher() { return std::shared_ptr<IDispatcher>(nullptr); };
};
