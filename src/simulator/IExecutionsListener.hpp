#pragma once

#include <utility>

#include "Operation.hpp"
#include "Types.hpp"

class IExecutionsListener
{
public:
  virtual void handleNotification(const Assignation& notification) = 0;
};
