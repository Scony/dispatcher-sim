#include <cassert>
#include <algorithm>
#include <math.h>

#include "JSADispatcher.hpp"
#include "Solution.hpp"
#include "Algorithm.hpp"

JSADispatcher::JSADispatcher(std::shared_ptr<Input> input,
			     std::shared_ptr<Cloud> cloud,
			     std::shared_ptr<IEstimator> estimator,
			     std::string operationLevelAlgorithm,
			     unsigned iterations) :
  Dispatcher(input, cloud, estimator),
  mOperationLevelAlgorithm(operationLevelAlgorithm),
  mIterations(iterations)
{
}

void JSADispatcher::dispatch(std::shared_ptr<Job> job)
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

  auto costFunction = [&](const std::vector<long long>& solution)
    {
      std::vector<OperationSP> opPermutation;
      for (auto const& jobId : solution)
	opPermutation.insert(opPermutation.end(),
			     mJobOperations[jobId].begin(),
			     mJobOperations[jobId].end());

      return ::Solution::evalTotalFlow(mCloud->simulate(mEstimator, opPermutation));
    };
  auto neighbouringSolution = [](std::vector<long long>& solution)
    {
      std::swap(solution[rand() % solution.size()],
		solution[rand() % solution.size()]);
    };

  mCurrentSolution = Algorithm::sa<std::vector<long long>, long long>(mCurrentSolution,
								      costFunction,
								      neighbouringSolution,
								      mIterations);
}

OperationSP JSADispatcher::pop()
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

size_t JSADispatcher::size()
{
  size_t totalSize = 0;
  for (const auto& kv : mJobOperations)
    totalSize += kv.second.size();

  return totalSize;
}
