#include "ELODispatcher.hpp"

ELODispatcher::ELODispatcher(std::shared_ptr<Input> input,
			   std::shared_ptr<Cloud> cloud,
			   std::shared_ptr<IEstimator> estimator) :
  Dispatcher(input, cloud, estimator)
{
}

void ELODispatcher::dispatch(JobSP job)
{
  for (const auto& operation : job->operations)
    mOperations.push(operation);
}

OperationSP ELODispatcher::pop()
{
  auto ret = mOperations.top();
  mOperations.pop();
  return ret;
}

size_t ELODispatcher::size()
{
  return mOperations.size();
}
