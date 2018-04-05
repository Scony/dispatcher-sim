#pragma once

#include "Operation.hpp"

class IQueue
{
public:
  virtual OperationSP peek() = 0;
  virtual OperationSP pop() = 0;
  virtual size_t size() = 0;
};
