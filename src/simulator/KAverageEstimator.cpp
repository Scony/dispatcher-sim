#include <math.h>

#include "KAverageEstimator.hpp"

KAverageEstimator::KAverageEstimator(unsigned k) : mK(k) {}

void KAverageEstimator::handleNotification(const Assignation& notification)
{
  auto& finishedOperation = std::get<1>(notification);

  operationDurations.emplace_back(finishedOperation->duration);
  operationDurationsSum += finishedOperation->duration;

  if (operationDurations.size() > mK)
  {
    operationDurationsSum -= operationDurations.front();
    operationDurations.pop_front();
  }
}

long long KAverageEstimator::estimate(OperationSP operation)
{
  return ceil((double)operationDurationsSum / operationDurations.size());
}
