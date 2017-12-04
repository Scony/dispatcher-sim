#include "NoEstimator.hpp"

void NoEstimator::handleNotification(const std::pair<long long, OperationSP>& notification)
{
}

long long NoEstimator::estimate(OperationSP operation)
{
  return operation->duration;
}
