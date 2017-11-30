#include <algorithm>
#include <cassert>

#include "RandomDispatcher.hpp"

RandomDispatcher::RandomDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud) :
  IDispatcher(input, cloud),
  mQueue()
{
}

void RandomDispatcher::dispatch(JobSP job)
{
  mQueue.insert(mQueue.end(), job->operations.begin(), job->operations.end());
  std::random_shuffle(mQueue.begin(), mQueue.end());
}

OperationSP RandomDispatcher::pop()
{
  assert(mQueue.size() > 0);

  auto operation = mQueue.back();
  mQueue.pop_back();

  return operation;
}

size_t RandomDispatcher::size()
{
  return mQueue.size();
}
