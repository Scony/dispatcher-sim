#include <cassert>
#include <algorithm>

#include "SJSODispatcher.hpp"

SJSODispatcher::SJSODispatcher(std::shared_ptr<Input> input,
			       std::shared_ptr<ICloud> cloud,
			       std::shared_ptr<IEstimator> estimator) :
    SJLODispatcher(input, cloud, estimator)
{
}

void SJSODispatcher::dispatch(std::vector<JobSP> jobs)
{
  // append new operations to map
  for (const auto& job : jobs)
  {
    mJobOperations[job->id] = job->operations;
    std::sort(mJobOperations[job->id].begin(),
              mJobOperations[job->id].end(),
              [&](OperationSP a, OperationSP b) {
                return mEstimator->estimate(a) > mEstimator->estimate(b); // DESC
              });
  }

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
