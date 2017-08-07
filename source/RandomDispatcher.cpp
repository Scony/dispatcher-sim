#include <algorithm>

#include "RandomDispatcher.hpp"

RandomDispatcher::RandomDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud) :
  IDispatcher(input, cloud)
{
}

void RandomDispatcher::dispatch(std::shared_ptr<Job> job)
{
  auto operations = job->operations;
  std::random_shuffle(operations.begin(), operations.end());

  auto queue = mCloud->getQueue();
  queue->insert(queue->end(), operations.begin(), operations.end());
}
