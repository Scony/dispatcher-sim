#include <algorithm>

#include "RandomDispatcher.hpp"

RandomDispatcher::RandomDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud) :
  SimpleDispatcher(input, cloud)
{
}

void RandomDispatcher::dispatch(std::shared_ptr<Job> job, Queue queue)
{
  queue->insert(queue->end(), job->operations.begin(), job->operations.end());
  std::random_shuffle(queue->begin(), queue->end());
}
