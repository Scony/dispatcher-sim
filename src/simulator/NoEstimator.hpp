#pragma once

#include "IEstimator.hpp"

class NoEstimator : public IEstimator
{
 public:
  void handleNotification(const Assignation& notification) override;

  long long estimate(OperationSP operation) override;
};
