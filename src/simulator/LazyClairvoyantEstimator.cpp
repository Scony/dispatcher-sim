#include "LazyClairvoyantEstimator.hpp"

LazyClairvoyantEstimator::LazyClairvoyantEstimator() :
  mKnownOperations(),
  mTotalOperationsDurations(0),
  mExecutedOperationsNum(0)
{
}

void LazyClairvoyantEstimator::handleNotification(const std::pair<long long, OperationSP>& notification)
{
  mKnownOperations.insert(notification.second->id);
  mTotalOperationsDurations += notification.second->duration;
  mExecutedOperationsNum++;
}

long long LazyClairvoyantEstimator::estimate(OperationSP operation)
{
  bool operationIsKnown = mKnownOperations.find(operation->id) != mKnownOperations.end();
  if (operationIsKnown)
    return operation->duration;
  else if (mExecutedOperationsNum > 0)
    return (long long)((double)mTotalOperationsDurations / mExecutedOperationsNum);
  else
    return mDefaultEstimate;
}
