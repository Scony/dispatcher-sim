#include <cassert>

#include "JobSADispatcher.hpp"

JobSADispatcher::JobSADispatcher(std::shared_ptr<Input> input,
				 std::shared_ptr<Cloud> cloud,
				 std::shared_ptr<IEstimator> estimator) :
  Dispatcher(input, cloud, estimator),
  mSolution(new Solution)
{
  mCloud->subscribe(mSolution);
}

void JobSADispatcher::dispatch(std::shared_ptr<Job> job)
{
  // add job to job pool
  // run SA
  // reset mQueue
}

OperationSP JobSADispatcher::pop()
{
  assert(mQueue.size() > 0);

  auto operation = mQueue.back();
  mQueue.pop_back();

  return operation;
}

size_t JobSADispatcher::size()
{
  return mQueue.size();
}
