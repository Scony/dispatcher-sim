#pragma once

#include <unordered_map>
#include <queue>

#include "IEstimator.hpp"

class KRecentEstimator : public IEstimator
{
public:
  KRecentEstimator(unsigned k);

  void handleNotification(const Assignation& notification) override;

  long long estimate(OperationSP operation) override;

private:
  const long long mDefaultEstimate = 42;
  const long long mK;

  std::unordered_map<long long, std::deque<long long> > operationDurations;
  std::unordered_map<long long, long long> operationDurationSums;
};
