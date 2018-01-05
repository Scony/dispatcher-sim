#include "SODispatcher.hpp"

SODispatcher::SODispatcher(std::shared_ptr<Input> input,
			   std::shared_ptr<Cloud> cloud,
			   std::shared_ptr<IEstimator> estimator) :
  Dispatcher(input, cloud, estimator)
{
}

void SODispatcher::dispatch(JobSP job)
{
  for (const auto& operation : job->operations)
    mOperations.push(operation);
}

OperationSP SODispatcher::pop()
{
  auto ret = mOperations.top();
  mOperations.pop();
  return ret;
}

size_t SODispatcher::size()
{
  return mOperations.size();
}
