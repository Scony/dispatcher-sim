#include <cassert>
#include <climits>

#include "Cloud.hpp"
#include "NoEstimator.hpp"
#include "VectorQueue.hpp"

Cloud::Cloud(unsigned machinesNum) :
  mMachinesNum(machinesNum),
  mTimestamp(0),
  mQueue(nullptr),
  mMachines()
{
}

void Cloud::advance(long long toTimestamp)
{
  assert(mQueue != nullptr);
  static IEstimatorSP noEstimator(new NoEstimator);

  for (auto const& assignment : process(mTimestamp,
					toTimestamp,
					mMachinesNum,
					noEstimator,
					mQueue,
					mMachines))
    notify(assignment);

  mTimestamp = toTimestamp;
}

void Cloud::assignQueue(IQueue* queue)
{
  mQueue = queue;
}

std::vector<Assignation> Cloud::process(const long long& fromTimestamp,
					const long long& toTimestamp,
					const unsigned& machinesNum,
					IEstimatorSP estimator,
					IQueue* queue,
					Machines& machines)
{
  // assumption: machines finishing before or at fromTimestamp should be emptied
  assert(machines.size() == 0 || machines.top().first > fromTimestamp);

  std::vector<Assignation> result;
  long long timestamp = fromTimestamp;

  // fill machines
  while (machines.size() < machinesNum && queue->size() > 0)
    {
      auto operation = queue->pop();
      // assumption: no operations from future
      assert(operation->arrival <= timestamp);
      machines.emplace(timestamp + estimator->estimate(operation), operation);
    }

  // loop over finishing machines and fill them
  while (machines.size() > 0 && machines.top().first <= toTimestamp)
    {
      timestamp = machines.top().first;
      auto& finishedOperation = machines.top().second;
      result.emplace_back(timestamp, finishedOperation);
      machines.pop();

      if (queue->size() > 0 && timestamp < toTimestamp)
	{
	  auto operation = queue->pop();
	  // assumption: no operations from future
	  assert(operation->arrival <= timestamp);
	  machines.emplace(timestamp + estimator->estimate(operation), operation);
	}
    }

  return result;
}

std::vector<Assignation> Cloud::simulate(IEstimatorSP estimator,
					 std::vector<OperationSP> operations) const
{
  const auto& fromTimestamp = mTimestamp;
  long long toTimestamp = LLONG_MAX;
  auto queue = std::make_shared<VectorQueue>(operations);
  auto machinesCpy = mMachines;
  return process(fromTimestamp,
		 toTimestamp,
		 mMachinesNum,
		 estimator,
		 queue.get(),
		 machinesCpy);
}

std::vector<Assignation> Cloud::simulateWithFuture(IEstimatorSP estimator,
						   std::vector<OperationSP> operations) const
{
  if (operations.size() == 0)
    return {};

  std::vector<Assignation> result;
  std::vector<OperationSP> queue;
  auto queueWrapper = std::make_shared<VectorQueue>(queue);
  auto machinesCpy = mMachines;

  long long fromTimestamp = operations.back()->arrival;
  while (operations.size() > 0)
    {
      while (operations.size() > 0 && operations.back()->arrival <= fromTimestamp)
	{
	  queue.insert(queue.begin(), operations.back());
	  operations.pop_back();
	}
      long long toTimestamp = operations.size() > 0 ? operations.back()->arrival : LLONG_MAX;
      auto partialResult = process(fromTimestamp,
				   toTimestamp,
				   mMachinesNum,
				   estimator,
				   queueWrapper.get(),
				   machinesCpy);
      result.insert(result.end(), partialResult.begin(), partialResult.end());
      fromTimestamp = toTimestamp;
    }

  return result;
}

unsigned Cloud::getMachinesNum()
{
  return mMachinesNum;
}
