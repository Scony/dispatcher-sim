#include <algorithm>
#include <cassert>

#include "SJDispatcher.hpp"

void SJDispatcher::dispatch(JobSP job)
{
  mJobOperations[job->id] = job->operations;
  reorderJobOperations(mJobOperations[job->id]);
  updateJobWeights();
}

void SJDispatcher::reorderJobOperations(std::vector<OperationSP>& operations) {}

void SJDispatcher::updateJobWeights()
{
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
  std::sort(
      mJobsInOrder.begin(), mJobsInOrder.end(), std::greater<std::pair<long long, long long>>());
}

OperationSP SJDispatcher::peek()
{
  assert(size() > 0);

  const auto& jobToPopFrom = mJobsInOrder.back().second;

  return mJobOperations[jobToPopFrom].back();
}

OperationSP SJDispatcher::pop()
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

size_t SJDispatcher::size()
{
  size_t totalSize = 0;
  for (const auto& kv : mJobOperations)
    totalSize += kv.second.size();

  return totalSize;
}
