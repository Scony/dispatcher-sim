#include <algorithm>

#include "QWORSTDispatcher.hpp"
#include "Solution.hpp"

QWORSTDispatcher::QWORSTDispatcher(std::shared_ptr<Input> input,
				   std::shared_ptr<Cloud> cloud,
				   std::shared_ptr<IEstimator> estimator) :
  QOPTDispatcher(input, cloud, estimator)
{
  std::vector<OperationSP> permutation;
  for (auto const& job : input->getJobs())
    permutation.insert(permutation.end(), job->operations.begin(), job->operations.end());

  std::sort(permutation.begin(), permutation.end());

  auto worstPermutation = permutation;
  auto worstPermutationCost = Solution::evalTotalFlow(Cloud::process(cloud->getMachinesNum(),
								     permutation));

  while (std::next_permutation(permutation.begin(), permutation.end()))
    {
      auto permutationCost = Solution::evalTotalFlow(Cloud::process(cloud->getMachinesNum(),
								    permutation));
      if (permutationCost > worstPermutationCost)
	{
	  worstPermutation = permutation;
	  worstPermutationCost = permutationCost;
	}
    }

  mQueue = worstPermutation;
}
