#include <math.h>

#include "KRecentEstimator.hpp"

KRecentEstimator::KRecentEstimator(unsigned k) :
    mK(k)
{
}

void KRecentEstimator::handleNotification(const Assignation& notification)
{
  auto& finishedOperation = std::get<1>(notification);

  if (operationDurations.find(finishedOperation->name) == operationDurations.end()) // new operation
  {
    operationDurations.emplace(finishedOperation->name, std::deque<long long>{});
    operationDurationSums.emplace(finishedOperation->name, 0);
  }

  operationDurations[finishedOperation->name].emplace_back(finishedOperation->duration);
  operationDurationSums[finishedOperation->name] += finishedOperation->duration;

  if (operationDurations[finishedOperation->name].size() > mK)
  {
    operationDurationSums[finishedOperation->name] -=
	operationDurations[finishedOperation->name].front();
    operationDurations[finishedOperation->name].pop_front();
  }
}

long long KRecentEstimator::estimate(OperationSP operation)
{
  double estimatedDuration = mDefaultEstimate;

  if (operationDurations.find(operation->name) != operationDurations.end()) // not new operation
    estimatedDuration = (double)operationDurationSums[operation->name]
	/ operationDurations[operation->name].size();

  return (int)ceil(estimatedDuration);
}
