#include <climits>

#include <benchmark/benchmark.h>

#include "Algorithm.hpp"

static void SA(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::vector<int> initialSolution = {1,2,3,4,5,6,7,8,9};
    auto costFunction = [](const std::vector<int>& solution)
	{
	  int cost = 0;
	  for (size_t i = 0; i < solution.size(); i++)
	    cost += i * solution[i];
	  return cost;
	};
    auto neighbouringSolution = [](std::vector<int>& solution)
	{
	  std::swap(solution[rand() % solution.size()], solution[rand() % solution.size()]);
	};
    Algorithm::sa<std::vector<int>, int>(initialSolution,
                                         costFunction,
                                         neighbouringSolution,
                                         10000);
  }
}
BENCHMARK(SA);
