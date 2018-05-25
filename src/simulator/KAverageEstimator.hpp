#pragma once

#include <queue>

#include "IEstimator.hpp"

class KAverageEstimator : public IEstimator
{
 public:
  KAverageEstimator(unsigned k);

  void handleNotification(const Assignation& notification) override;

  long long estimate(OperationSP operation) override;

 private:
  const long long mDefaultEstimate = 42;
  const unsigned mK;

  std::deque<long long> operationDurations;
  long long operationDurationsSum = 0;
};
