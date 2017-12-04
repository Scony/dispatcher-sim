#pragma once

#include "IEstimator.hpp"

class NoEstimator : public IEstimator
{
public:
  void handleNotification(const std::pair<long long, OperationSP>& notification) override;
};
