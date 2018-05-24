#include "AverageEstimator.hpp"

void AverageEstimator::handleNotification(const Assignation& notification)
{
  auto& finishedOperation = std::get<1>(notification);
  mTotalOperationsDuration += finishedOperation->duration;
  mExecutedOperationsNum++;
}

long long AverageEstimator::estimate(OperationSP operation)
{
  return (long long)((double)mTotalOperationsDuration / mExecutedOperationsNum);
}
