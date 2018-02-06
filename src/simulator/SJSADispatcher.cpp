#include <algorithm>
#include <cassert>

#include "SJSADispatcher.hpp"

SJSADispatcher::SJSADispatcher(std::shared_ptr<Input> input,
			       std::shared_ptr<Cloud> cloud,
			       std::shared_ptr<IEstimator> estimator,
			       std::string operationLevelAlgorithm,
			       unsigned iterations) :
  SASADispatcher(input, cloud, estimator, operationLevelAlgorithm, iterations)
{
}

void SJSADispatcher::dispatch(std::shared_ptr<Job> job)
{
  mJobOperations[job->id] = job->operations;

  if (mOperationLevelAlgorithm == "random")
    std::random_shuffle(mJobOperations[job->id].begin(), mJobOperations[job->id].end());
  else if (mOperationLevelAlgorithm == "max")
    std::sort(mJobOperations[job->id].begin(),
	      mJobOperations[job->id].end(),
	      [&](OperationSP a, OperationSP b) {
		return a->duration < b->duration; // ASC
	      });
  else if (mOperationLevelAlgorithm == "min")
    std::sort(mJobOperations[job->id].begin(),
	      mJobOperations[job->id].end(),
	      [&](OperationSP a, OperationSP b) {
		return a->duration > b->duration; // DESC
	      });
  else
    assert(false);

  mCurrentSolution.insert(mCurrentSolution.begin(), job->id);

  std::vector<std::pair<long long, long long> > mWeightsOfJobs;
  for (const auto& jobId : mCurrentSolution)
    {
      long long jobWeight = 0;
      for (const auto& operation : mJobOperations[jobId])
	jobWeight += mEstimator->estimate(operation);

      mWeightsOfJobs.push_back({jobWeight, jobId});
    }
  std::sort(mWeightsOfJobs.begin(),
	    mWeightsOfJobs.end(),
	    std::greater<std::pair<long long, long long> >());

  mCurrentSolution.clear();
  for (const auto& pair : mWeightsOfJobs)
    mCurrentSolution.push_back(pair.second);

  operationLevelSA();
}
