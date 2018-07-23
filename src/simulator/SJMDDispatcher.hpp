#pragma once

#include <algorithm>

#include "SJDispatcher.hpp"

class SJMDDispatcher : public SJDispatcher
{
 public:
  using SJDispatcher::SJDispatcher;

  virtual void reorderJobOperations(std::vector<OperationSP>& operations) override
  {
    auto operationsInOrder = operations;
    std::sort(operationsInOrder.begin(),
              operationsInOrder.end(),
              [&](OperationSP a, OperationSP b) {
                return mEstimator->estimate(a) < mEstimator->estimate(b); // ASC
              });

    operations.clear();
    while (!operationsInOrder.empty())
    {
      auto median = operationsInOrder.size() / 2;
      operations.push_back(operationsInOrder[median]);
      operationsInOrder.erase(operationsInOrder.begin() + median);
    }
    std::reverse(operations.begin(), operations.end());
  }
};
