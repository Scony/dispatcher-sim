#include <math.h>

#include "SASADispatcher.hpp"
#include "Solution.hpp"

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
  JSADispatcher::dispatch(job);
  sa2();
}

long long SASADispatcher::eval2(std::map<long long, std::vector<OperationSP> >& jobOperations)
{
  std::vector<OperationSP> opPermutation;
  for (auto const& jobId : mCurrentSolution)
    opPermutation.insert(opPermutation.end(),
  			 jobOperations[jobId].begin(),
  			 jobOperations[jobId].end());

  return ::Solution::evalTotalFlow(mCloud->simulate(mEstimator, opPermutation));
}

void SASADispatcher::sa2()
{
  std::map<long long, std::vector<OperationSP> > bestSolution = mJobOperations;
  long long bestCost = eval(mCurrentSolution);

  std::map<long long, std::vector<OperationSP> > prevSolution = bestSolution;
  long long prevCost = bestCost;

  double T = 1.0;
  for (unsigned i = 0; i < mIterations; i++)
    {
      // calculate temperature
      T = 1.0 - ((double)i / mIterations);

      // prepare new candidate
      auto candidate = prevSolution;
      auto jobId = mCurrentSolution[rand() % mCurrentSolution.size()];
      std::swap(candidate[jobId][rand() % candidate[jobId].size()],
		candidate[jobId][rand() % candidate[jobId].size()]);
      long long candidateCost = eval2(candidate);

      // calculate acceptance probability
      double ap;
      if (candidateCost < prevCost)
	ap = 1.0;
      else
	ap = exp((double)(prevCost - candidateCost) / T);

      if (candidateCost < bestCost)
	{
	  bestSolution = candidate;
	  bestCost = candidateCost;
	}

      if (ap >= ((double)((rand() % 1000)+1)/1000))
	{
	  prevSolution = candidate;
	  prevCost = candidateCost;
	}
    }

  mJobOperations = bestSolution;
}
