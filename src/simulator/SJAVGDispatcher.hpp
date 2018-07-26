#pragma once

#include <algorithm>
#include <numeric>

#include "SJDispatcher.hpp"

class SJAVGDispatcher : public SJDispatcher
{
 public:
  using SJDispatcher::SJDispatcher;

  virtual void reorderJobOperations(std::vector<OperationSP>& operations) override
  {
    auto operationsCpy = operations;
    operations.clear();
    while (!operationsCpy.empty())
    {
      auto sum = std::accumulate(
          operationsCpy.begin(),
          operationsCpy.end(),
          0,
          [&](const long long& sum, const OperationSP& operation) {
            return sum + mEstimator->estimate(operation);
          });
      double avg = (double)sum / operationsCpy.size();
      std::size_t closestIx = -1;
      double closestDistance = std::numeric_limits<double>::max();
      for (auto i = 0u; i < operationsCpy.size(); i++)
      {
        auto distance = abs((double)mEstimator->estimate(operationsCpy[i]) - avg);
        if (distance < closestDistance)
        {
          closestDistance = distance;
          closestIx = i;
        }
      }
      operations.push_back(operationsCpy[closestIx]);
      operationsCpy.erase(operationsCpy.begin() + closestIx);
    }
    std::reverse(operations.begin(), operations.end());
  }
};
