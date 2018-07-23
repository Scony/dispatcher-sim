#include "SODispatcher.hpp"

SODispatcher::SODispatcher(std::shared_ptr<Input> input,
			   std::shared_ptr<ICloud> cloud,
			   std::shared_ptr<IEstimator> estimator) :
    Dispatcher(input, cloud, estimator)
{
}

void SODispatcher::dispatch(std::vector<JobSP> jobs)
{
  for (const auto& job : jobs)
    for (const auto& operation : job->operations)
      mOperations.push(operation);
}

OperationSP SODispatcher::peek()
{
  return mOperations.top();
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
