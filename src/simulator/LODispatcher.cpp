#include "LODispatcher.hpp"

LODispatcher::LODispatcher(std::shared_ptr<Input> input,
			   std::shared_ptr<ICloud> cloud,
			   std::shared_ptr<IEstimator> estimator) :
  Dispatcher(input, cloud, estimator)
{
}

void LODispatcher::dispatch(JobSP job)
{
  for (const auto& operation : job->operations)
    mOperations.push(operation);
}

OperationSP LODispatcher::peek()
{
  return mOperations.top();
}

OperationSP LODispatcher::pop()
{
  auto ret = mOperations.top();
  mOperations.pop();
  return ret;
}

size_t LODispatcher::size()
{
  return mOperations.size();
}
