#include <math.h>

#include "SADispatcher.hpp"
#include "Solution.hpp"

SADispatcher::SADispatcher(std::shared_ptr<Input> input,
			   std::shared_ptr<Cloud> cloud,
			   std::shared_ptr<IEstimator> estimator,
			   unsigned iterations) :
  RandomDispatcher(input, cloud, estimator),
  mIterations(iterations)
{
}

void SADispatcher::dispatch(JobSP job)
{
  mQueue.insert(mQueue.end(), job->operations.begin(), job->operations.end());
  sa();
}

void SADispatcher::sa()
{
  auto bestSolution = mQueue;
  long long bestCost = ::Solution::evalTotalFlow(mCloud->simulate(mEstimator, bestSolution));

  auto prevSolution = bestSolution;
  long long prevCost = bestCost;

  double T = 1.0;
  for (unsigned i = 0; i < mIterations; i++)
    {
      // calculate temperature
      T = 1.0 - ((double)i / mIterations);

      // prepare new candidate
      auto candidate = prevSolution;
      std::swap(candidate[rand() % candidate.size()],
		candidate[rand() % candidate.size()]);
      long long candidateCost = ::Solution::evalTotalFlow(mCloud->simulate(mEstimator, candidate));

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

  mQueue = bestSolution;
}
