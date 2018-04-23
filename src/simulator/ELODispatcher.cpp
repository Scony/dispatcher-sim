#include "ELODispatcher.hpp"

ELODispatcher::ELODispatcher(std::shared_ptr<Input> input,
                             std::shared_ptr<ICloud> cloud,
                             std::shared_ptr<IEstimator> estimator) :
    Dispatcher(input, cloud, estimator)
{
}

void ELODispatcher::dispatch(JobSP job)
{
  for (const auto& operation : job->operations)
    mOperations.push(operation);
}

OperationSP ELODispatcher::peek()
{
  return mOperations.top();
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
