#include <cassert>

#include "JobSADispatcher.hpp"

JobSADispatcher::JobSADispatcher(std::shared_ptr<Input> input,
				 std::shared_ptr<Cloud> cloud,
				 std::shared_ptr<IEstimator> estimator) :
  Dispatcher(input, cloud, estimator)
{
}

void JobSADispatcher::dispatch(std::shared_ptr<Job> job)
{
  mJobOperations[job->id] = {};	// job->operations;
  mJobOperations[job->id].insert(mJobOperations[job->id].end(),
				 job->operations.rbegin(),
				 job->operations.rend());
  mCurrentSolution.insert(mCurrentSolution.begin(), job->id);

  // TODO: run SA to fix mCurrentSolution
}

OperationSP JobSADispatcher::pop()
{
  assert(mJobOperations.size() > 0);

  auto jobId = mCurrentSolution.back();
  auto operation = mJobOperations[jobId].back();
  mJobOperations[jobId].pop_back();

  if (mJobOperations[jobId].size() == 0)
    {
      mJobOperations.erase(jobId);
      mCurrentSolution.pop_back();
    }

  return operation;
}

size_t JobSADispatcher::size()
{
  size_t totalSize = 0;
  for (const auto& kv : mJobOperations)
    totalSize += kv.second.size();

  return totalSize;
}
