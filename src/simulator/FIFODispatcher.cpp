#include <cassert>

#include "FIFODispatcher.hpp"

FIFODispatcher::FIFODispatcher(std::shared_ptr<Input> input,
			       std::shared_ptr<Cloud> cloud,
			       std::shared_ptr<IEstimator> estimator) :
  Dispatcher(input, cloud, estimator),
  mOperations()
{
}

void FIFODispatcher::dispatch(JobSP job)
{
  mOperations.insert(mOperations.end(), job->operations.begin(), job->operations.end());
}

OperationSP FIFODispatcher::pop()
{
  assert(size() > 0);

  auto operationToReturn = mOperations.front();
  mOperations.pop_front();

  return operationToReturn;
}

size_t FIFODispatcher::size()
{
  return mOperations.size();
}
