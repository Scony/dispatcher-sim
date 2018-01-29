#pragma once

#include "Operation.hpp"

class IQueue
{
public:
  virtual OperationSP pop() = 0;
  virtual size_t size() = 0;
};
