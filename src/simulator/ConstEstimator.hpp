#pragma once

#include "IEstimator.hpp"

class ConstEstimator : public IEstimator
{
 public:
  void handleNotification(const Assignation& notification) override {}

  long long estimate(OperationSP operation) override { return mConstEstimate; }

 private:
  const long long mConstEstimate = 1;
};
