#include "NoEstimator.hpp"

void NoEstimator::handleNotification(const Assignation& notification)
{
}

long long NoEstimator::estimate(OperationSP operation)
{
  return operation->duration;
}
