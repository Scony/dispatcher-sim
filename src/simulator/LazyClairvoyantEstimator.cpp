#include "LazyClairvoyantEstimator.hpp"

LazyClairvoyantEstimator::LazyClairvoyantEstimator() :
    mKnownOperations(),
    mTotalOperationsDurations(0),
    mExecutedOperationsNum(0)
{
}

void LazyClairvoyantEstimator::handleNotification(const Assignation& notification)
{
  auto& finishedOperation = std::get<1>(notification);
  mKnownOperations.insert(finishedOperation->id);
  mTotalOperationsDurations += finishedOperation->duration;
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
