#include <algorithm>

#include "RandomDispatcher.hpp"

RandomDispatcher::RandomDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud) :
  IDispatcher(input, cloud)
{
}

void RandomDispatcher::dispatch(JobSP job)
{
  // queue->insert(queue->end(), job->operations.begin(), job->operations.end());
  // std::random_shuffle(queue->begin(), queue->end());
}
