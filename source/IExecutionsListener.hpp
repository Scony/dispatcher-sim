#pragma once

#include <utility>

#include "Operation.hpp"

class IExecutionsListener
{
public:
  virtual void handleNotification(const std::pair<long long, OperationSP>& notification) = 0;
};
