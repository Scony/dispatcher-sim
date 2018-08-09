#include <cmath>
#include <algorithm>
#include <map>

#include "SADispatcher.hpp"
#include "Solution.hpp"
#include "Algorithm.hpp"

SADispatcher::SADispatcher(std::shared_ptr<Input> input,
			   std::shared_ptr<ICloud> cloud,
			   std::shared_ptr<IEstimator> estimator,
			   unsigned iterations) :
    RandomDispatcher(input, cloud, estimator),
    mIterations(iterations)
{
}

void SADispatcher::dispatch(std::vector<JobSP> jobs)
{
  for (const auto& job : jobs)
    mQueue.insert(mQueue.end(), job->operations.begin(), job->operations.end());

  std::map<long long, long long> jobWeights;
  for (const auto& operation : mQueue)
  {
    if (jobWeights.find(operation->parentId) == jobWeights.end())
      jobWeights[operation->parentId] = 0;
    jobWeights[operation->parentId] += mEstimator->estimate(operation);
  }

  std::sort(mQueue.begin(),
            mQueue.end(),
            [&](OperationSP a, OperationSP b) {
              if (a->parentId != b->parentId)
                return jobWeights[a->parentId] > jobWeights[b->parentId]; // SJ
              return mEstimator->estimate(a) < mEstimator->estimate(b); // LO
            });

  auto costFunction = [&](const std::vector<OperationSP>& solution)
      {
        return ::Solution::evalTotalFlow(mCloud->simulate(mEstimator, solution));
      };
  auto neighbouringSolution = [](std::vector<OperationSP>& solution)
      {
        std::swap(solution[rand() % solution.size()],
                  solution[rand() % solution.size()]);
      };

  mQueue = algorithm::sa<std::vector<OperationSP>, long long>(mQueue,
							      costFunction,
							      neighbouringSolution,
							      mIterations);
}
