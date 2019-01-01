#include <algorithm>
#include <cassert>
#include <iostream>

#include "JOPTDispatcher.hpp"
#include "NoEstimator.hpp"
#include "Solution.hpp"

JOPTDispatcher::JOPTDispatcher(
    std::shared_ptr<Input> input,
    std::shared_ptr<ICloud> cloud,
    std::shared_ptr<IEstimator> estimator,
    std::string operationLevelAlgorithm)
    : QOPTDispatcher(input, cloud, estimator, false)
{
  std::vector<JobSP> jobPermutation;
  std::map<long long, std::vector<OperationSP>> jobOperations;
  for (auto const& job : input->getJobs())
  {
    jobPermutation.push_back(job);
    jobOperations[job->id] = job->operations;

    if (operationLevelAlgorithm == "random")
      std::random_shuffle(jobOperations[job->id].begin(), jobOperations[job->id].end());
    else if (operationLevelAlgorithm == "max")
      std::sort(
          jobOperations[job->id].begin(),
          jobOperations[job->id].end(),
          [&](OperationSP a, OperationSP b) {
            return a->duration < b->duration; // ASC
          });
    else if (operationLevelAlgorithm == "min")
      std::sort(
          jobOperations[job->id].begin(),
          jobOperations[job->id].end(),
          [&](OperationSP a, OperationSP b) {
            return a->duration > b->duration; // DESC
          });
    else
      assert(false);
  }

  std::sort(jobPermutation.begin(), jobPermutation.end());
  std::vector<OperationSP> opPermutation;
  for (auto const& job : jobPermutation)
    opPermutation.insert(
        opPermutation.end(), jobOperations[job->id].begin(), jobOperations[job->id].end());

  auto bestOpPermutation = opPermutation;
  auto bestOpPermutationCost = Solution::evalTotalFlow(
      mCloud->simulateWithFuture(std::make_shared<NoEstimator>(), opPermutation));
  while (std::next_permutation(jobPermutation.begin(), jobPermutation.end()))
  {
    static unsigned iterations = 1;

    opPermutation.clear();
    for (auto const& job : jobPermutation)
      opPermutation.insert(
          opPermutation.end(), jobOperations[job->id].begin(), jobOperations[job->id].end());
    auto opPermutationCost = Solution::evalTotalFlow(
        mCloud->simulateWithFuture(std::make_shared<NoEstimator>(), opPermutation));

    if (opPermutationCost < bestOpPermutationCost)
    {
      bestOpPermutation = opPermutation;
      bestOpPermutationCost = opPermutationCost;
    }

    iterations++;
    if ((iterations % 100000) == 0)
      std::cerr << iterations << std::endl;
  }

  mQueue = bestOpPermutation;
}
