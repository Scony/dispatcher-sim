#include <cassert>
#include <algorithm>

#include "SJLODispatcher.hpp"

SJLODispatcher::SJLODispatcher(std::shared_ptr<Input> input,
			       std::shared_ptr<Cloud> cloud,
			       std::shared_ptr<IEstimator> estimator) :
  Dispatcher(input, cloud, estimator),
  mJobOperations(),
  mJobsInOrder()
{
}

void SJLODispatcher::dispatch(JobSP job)
{
  // append new operations to map
  mJobOperations[job->id] = job->operations;
  std::sort(mJobOperations[job->id].begin(),
  	    mJobOperations[job->id].end(),
  	    [&](OperationSP a, OperationSP b) {
  	      return mEstimator->estimate(a) < mEstimator->estimate(b); // ASC
  	    });

  // update weights of jobs
  mJobsInOrder.clear();
  for (const auto& kv : mJobOperations)
    {
      const auto& jobId = kv.first;
      const auto& remainingJobOperations = kv.second;

      long long jobWeight = 0;
      for (const auto& operation : remainingJobOperations)
	jobWeight += mEstimator->estimate(operation);

      mJobsInOrder.push_back({jobWeight, jobId});
    }
  std::sort(mJobsInOrder.begin(), mJobsInOrder.end(), std::greater<std::pair<long long, long long> >());
}

OperationSP SJLODispatcher::peek()
{
  assert(size() > 0);

  const auto& jobToPopFrom = mJobsInOrder.back().second;

  return mJobOperations[jobToPopFrom].back();
}

OperationSP SJLODispatcher::pop()
{
  assert(size() > 0);

  const auto& jobToPopFrom = mJobsInOrder.back().second;

  auto operation = mJobOperations[jobToPopFrom].back();
  mJobOperations[jobToPopFrom].pop_back();

  if (mJobOperations[jobToPopFrom].size() == 0)
    {
      mJobOperations.erase(jobToPopFrom);
      mJobsInOrder.pop_back();
    }

  return operation;
}

size_t SJLODispatcher::size()
{
  size_t totalSize = 0;
  for (const auto& kv : mJobOperations)
    totalSize += kv.second.size();

  return totalSize;
}
