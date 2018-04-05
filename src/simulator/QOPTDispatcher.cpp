#include <cassert>
#include <algorithm>

#include "QOPTDispatcher.hpp"
#include "Solution.hpp"
#include "NoEstimator.hpp"

QOPTDispatcher::QOPTDispatcher(std::shared_ptr<Input> input,
			       std::shared_ptr<Cloud> cloud,
			       std::shared_ptr<IEstimator> estimator,
			       bool calculateSolution) :
  Dispatcher(input, cloud, estimator),
  mQueue(),
  mTime(-1)
{
  if (!calculateSolution)
    return;

  std::vector<OperationSP> permutation;
  for (auto const& job : input->getJobs())
    permutation.insert(permutation.end(), job->operations.begin(), job->operations.end());

  std::sort(permutation.begin(), permutation.end());

  auto bestPermutation = permutation;
  auto bestPermutationCost =
    Solution::evalTotalFlow(mCloud->simulateWithFuture(std::make_shared<NoEstimator>(),
						       permutation));

  while (std::next_permutation(permutation.begin(), permutation.end()))
    {
      auto permutationCost =
	Solution::evalTotalFlow(mCloud->simulateWithFuture(std::make_shared<NoEstimator>(),
							   permutation));
      if (permutationCost < bestPermutationCost)
	{
	  bestPermutation = permutation;
	  bestPermutationCost = permutationCost;
	}
    }

  mQueue = bestPermutation;
}

void QOPTDispatcher::dispatch(JobSP job)
{
  mTime = job->arrivalTimestamp;
}

OperationSP QOPTDispatcher::peek()
{
  assert(size() > 0);

  return mQueue.back();
}

OperationSP QOPTDispatcher::pop()
{
  assert(size() > 0);

  auto operation = mQueue.back();
  mQueue.pop_back();

  return operation;
}

size_t QOPTDispatcher::size()
{
  long long operationsReadyToGo = 0;

  for (int i = mQueue.size() - 1; i >= 0; i--)
    if (mQueue[i]->arrival <= mTime)
      operationsReadyToGo++;
    else
      break;

  return operationsReadyToGo;
}
