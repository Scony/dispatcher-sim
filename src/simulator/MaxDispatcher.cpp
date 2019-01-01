#include <algorithm>
#include <cassert>
#include <map>

#include "MaxDispatcher.hpp"

MaxDispatcher::MaxDispatcher(
    std::shared_ptr<Input> input,
    std::shared_ptr<ICloud> cloud,
    std::shared_ptr<IEstimator> estimator)
    : Dispatcher(input, cloud, estimator), mJobOperations(), mNextJob(0)
{
}

void MaxDispatcher::dispatch(JobSP job)
{
  mJobOperations[job->id] = job->operations;
  std::sort(
      mJobOperations[job->id].begin(),
      mJobOperations[job->id].end(),
      [&](OperationSP a, OperationSP b) {
        return mEstimator->estimate(a) < mEstimator->estimate(b); // ASC
      });
}

OperationSP MaxDispatcher::peek()
{
  assert(size() > 0);

  auto jobToPopFrom = mJobOperations.begin();
  std::advance(jobToPopFrom, mNextJob);

  return jobToPopFrom->second.back();
}

OperationSP MaxDispatcher::pop()
{
  assert(size() > 0);

  auto jobToPopFrom = mJobOperations.begin();
  std::advance(jobToPopFrom, mNextJob);

  auto operation = jobToPopFrom->second.back();
  jobToPopFrom->second.pop_back();

  if (jobToPopFrom->second.size() == 0)
    mJobOperations.erase(jobToPopFrom);

  if (mJobOperations.size() == 0)
    mNextJob = 0;
  else
    mNextJob = (mNextJob + 1) % mJobOperations.size();

  return operation;
}

size_t MaxDispatcher::size()
{
  size_t totalSize = 0;
  for (const auto& kv : mJobOperations)
    totalSize += kv.second.size();

  return totalSize;
}
