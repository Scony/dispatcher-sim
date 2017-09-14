#include "SimpleDispatcher.hpp"

SimpleDispatcher::SimpleDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud) :
  IDispatcher(input, cloud)
{
}

void SimpleDispatcher::dispatch(std::shared_ptr<Job> job)
{
  auto queue = mCloud->getQueue();
  std::vector<std::shared_ptr<Operation> > pendingOperations;

  pendingOperations.insert(pendingOperations.end(), queue->begin(), queue->end());
  queue->clear();

  dispatch(job, pendingOperations, queue);
}
