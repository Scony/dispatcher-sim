#pragma once

#include "IExecutionsListener.hpp"
#include "Operation.hpp"

class IEstimator : public IExecutionsListener
{
public:
  virtual long long estimate(OperationSP operation) = 0;
};
