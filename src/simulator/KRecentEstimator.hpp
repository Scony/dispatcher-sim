#pragma once

#include <queue>
#include <unordered_map>

#include "IEstimator.hpp"

class KRecentEstimator : public IEstimator
{
 public:
  KRecentEstimator(unsigned k);

  void handleNotification(const Assignation& notification) override;

  long long estimate(OperationSP operation) override;

 private:
  const long long mDefaultEstimate = 42;
  const unsigned mK;

  std::unordered_map<long long, std::deque<long long>> operationDurations;
  std::unordered_map<long long, long long> operationDurationSums;
};
