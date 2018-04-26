#pragma once

#include <functional>
#include <math.h>
#include <stdlib.h>

namespace Algorithm
{
template <typename Solution, typename Cost>
Solution sa(const Solution& initialSolution,
            std::function<Cost(const Solution&)> costFunction,
            std::function<void(Solution&)> neighbouringSolution,
            unsigned iterations)
{
  Solution bestSolution = initialSolution;
  Cost bestCost = costFunction(bestSolution);

  Solution prevSolution = bestSolution;
  Cost prevCost = bestCost;

  float T = 1.0f;
  for (unsigned i = 0; i < iterations; i++)
  {
    // calculate temperature
    T = 1.0f - ((float)i / iterations);

    // prepare new candidate
    Solution candidate = prevSolution;
    neighbouringSolution(candidate);
    Cost candidateCost = costFunction(candidate);

    if (candidateCost < bestCost)
    {
      bestSolution = candidate;
      bestCost = candidateCost;
      prevSolution = candidate;
      prevCost = candidateCost;
    }
    else if (candidateCost < prevCost)
    {
      prevSolution = candidate;
      prevCost = candidateCost;
    }
    else
    {
      // calculate acceptance probability
      float ap = exp((float)(prevCost - candidateCost) / T);
      if (ap >= ((float)((rand() % 1000) + 1) / 1000))
      {
        prevSolution = candidate;
        prevCost = candidateCost;
      }
    }
  }

  return bestSolution;
}

template <typename Solution, typename Cost>
void sa_inplace(Solution& initialSolution,
                std::function<Cost(const Solution&)> costFunction,
                std::function<void(Solution&)> invertSolution,
                std::function<void(Solution&)> revertSolution,
                unsigned iterations)
{
  Solution bestSolution = initialSolution;
  Cost bestCost = costFunction(bestSolution);

  Solution& prevSolution = initialSolution;
  Cost prevCost = bestCost;

  float T = 1.0f;
  for (unsigned i = 0; i < iterations; i++)
  {
    // calculate temperature
    T = 1.0f - ((float)i / iterations);

    // prepare new candidate
    Solution& candidate = prevSolution;
    invertSolution(candidate);
    Cost candidateCost = costFunction(candidate);

    if (candidateCost < bestCost)
    {
      bestSolution = candidate;
      bestCost = candidateCost;
      prevCost = candidateCost;
    }
    else if (candidateCost < prevCost)
    {
      prevCost = candidateCost;
    }
    else
    {
      // calculate acceptance probability
      float ap = exp((float)(prevCost - candidateCost) / T);
      if (ap >= ((float)((rand() % 1000) + 1) / 1000))
        prevCost = candidateCost;
      else
        revertSolution(candidate);
    }
  }

  initialSolution = bestSolution;
}
}
