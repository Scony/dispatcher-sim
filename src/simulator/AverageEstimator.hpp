#pragma once

#include "IEstimator.hpp"

class AverageEstimator : public IEstimator
{
 public:
  // AverageEstimator.hpp
  void handleNotification(const Assignation& notification) override;

  long long estimate(OperationSP operation) override;
 private:
  long long mTotalOperationsDuration = 0;
  long long mExecutedOperationsNum = 0;
};
