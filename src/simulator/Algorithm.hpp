#pragma once

#include <functional>
#include <math.h>

namespace Algorithm
{
  template <typename Solution, typename Cost>
  Solution sa(Solution initialSolution,
	      std::function<Cost(const Solution&)> costFunction,
	      std::function<void(Solution&)> neighbouringSolution,
	      unsigned iterations)
  {
    Solution bestSolution = initialSolution;
    Cost bestCost = costFunction(bestSolution);

    Solution prevSolution = bestSolution;
    Cost prevCost = bestCost;

    double T = 1.0;
    for (unsigned i = 0; i < iterations; i++)
      {
	// calculate temperature
	T = 1.0 - ((double)i / iterations);

	// prepare new candidate
	Solution candidate = prevSolution;
	neighbouringSolution(candidate);
	Cost candidateCost = costFunction(candidate);

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

    return bestSolution;
  }
}
