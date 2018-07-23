#pragma once

#include <algorithm>

#include "SJDispatcher.hpp"

class SJSODispatcher : public SJDispatcher
{
 public:
  using SJDispatcher::SJDispatcher;

  void reorderJobOperations(std::vector<OperationSP>& operations)
  {
    std::sort(operations.begin(),
              operations.end(),
              [&](OperationSP a, OperationSP b) {
                return mEstimator->estimate(a) > mEstimator->estimate(b); // DESC
              });
  }
};
