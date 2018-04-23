#include <gtest/gtest.h>

#include "Algorithm.hpp"

TEST(AlgorithmTests, SA1)
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

  auto saSolution = Algorithm::sa<std::vector<int>, int>(initialSolution,
							 costFunction,
							 neighbouringSolution,
							 1000);
  EXPECT_LT(costFunction(saSolution), costFunction(initialSolution));
}
