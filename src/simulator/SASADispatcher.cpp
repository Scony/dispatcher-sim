#include <math.h>

#include "SASADispatcher.hpp"
#include "Solution.hpp"
#include "Algorithm.hpp"

SASADispatcher::SASADispatcher(std::shared_ptr<Input> input,
			       std::shared_ptr<Cloud> cloud,
			       std::shared_ptr<IEstimator> estimator,
			       std::string operationLevelAlgorithm,
			       unsigned iterations) :
  JSADispatcher(input, cloud, estimator, operationLevelAlgorithm, iterations)
{
}

void SASADispatcher::dispatch(std::shared_ptr<Job> job)
{
  JSADispatcher::dispatch(job);	// job SA here
  operationLevelSA();
}

void SASADispatcher::operationLevelSA()
{
  auto costFunction = [&](const std::map<long long, std::vector<OperationSP> >& solution)
    {
      std::vector<OperationSP> opPermutation;
      for (auto const& jobId : mCurrentSolution)
	opPermutation.insert(opPermutation.end(),
			     solution.at(jobId).begin(),
			     solution.at(jobId).end());

      return ::Solution::evalTotalFlow(mCloud->simulate(mEstimator, opPermutation));
    };
  auto neighbouringSolution = [&](std::map<long long, std::vector<OperationSP> >& solution)
    {
      auto jobId = mCurrentSolution[rand() % mCurrentSolution.size()];
      std::swap(solution[jobId][rand() % solution[jobId].size()],
		solution[jobId][rand() % solution[jobId].size()]);
    };

  mJobOperations = Algorithm::sa<std::map<long long, std::vector<OperationSP> >,
				 long long>(mJobOperations,
					    costFunction,
					    neighbouringSolution,
					    mIterations);
}
