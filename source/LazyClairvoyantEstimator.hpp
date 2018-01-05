#pragma once

#include <set>

#include "IEstimator.hpp"

class LazyClairvoyantEstimator : public IEstimator
{
public:
  LazyClairvoyantEstimator();

  void handleNotification(const std::pair<long long, OperationSP>& notification) override;

  long long estimate(OperationSP operation) override;

private:
  const long long mDefaultEstimate = 42;

  std::set<long long> mKnownOperations;
  long long mTotalOperationsDurations;
  long long mExecutedOperationsNum;
};
