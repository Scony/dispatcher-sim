#pragma once

#include "IExecutionsListener.hpp"
#include "Operation.hpp"

class IEstimator : public IExecutionsListener
{
public:
  virtual long long estimate(OperationSP operation) = 0;
};

using IEstimatorSP = std::shared_ptr<IEstimator>;
