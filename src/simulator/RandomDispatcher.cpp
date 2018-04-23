#include <algorithm>
#include <cassert>

#include "RandomDispatcher.hpp"

RandomDispatcher::RandomDispatcher(std::shared_ptr<Input> input,
				   std::shared_ptr<ICloud> cloud,
				   std::shared_ptr<IEstimator> estimator) :
    Dispatcher(input, cloud, estimator),
    mQueue()
{
}

void RandomDispatcher::dispatch(JobSP job)
{
  mQueue.insert(mQueue.end(), job->operations.begin(), job->operations.end());
  std::random_shuffle(mQueue.begin(), mQueue.end());
}

OperationSP RandomDispatcher::peek()
{
  assert(mQueue.size() > 0);

  return mQueue.back();
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
