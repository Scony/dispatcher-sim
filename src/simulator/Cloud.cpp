#include <cassert>
#include <climits>

#include "Cloud.hpp"
#include "NoEstimator.hpp"
#include "VectorQueue.hpp"

Cloud::Cloud(unsigned machinesNum) :
  mMachinesNum(machinesNum),
  mTimestamp(0),
  mQueue(nullptr),
  mBusyMachines(),
  mAssignationsCounter(0)
{
  for (unsigned machineId = 0; machineId < machinesNum; machineId++)
    mFreeMachines.emplace_front(machineId, -1);
}

std::vector<Assignation> Cloud::process(const long long& fromTimestamp,
					const long long& toTimestamp,
					const unsigned& machinesNum,
					IEstimatorSP estimator,
					IQueue* queue,
					FreeMachines& freeMachines,
					BusyMachines& busyMachines,
					long long& assignationsCounter)
{
  // assumption: machines finishing before or at fromTimestamp should be emptied
  assert(busyMachines.size() == 0 || std::get<0>(busyMachines.top()) > fromTimestamp);

  std::vector<Assignation> result;
  long long timestamp = fromTimestamp;

  // fill machines
  while (busyMachines.size() < machinesNum && queue->size() > 0)
    {
      auto operation = queue->pop();
      // assumption: no operations from future
      assert(operation->arrival <= timestamp);
      busyMachines.emplace(timestamp + estimator->estimate(operation),
			   ++assignationsCounter,
			   operation,
			   std::get<0>(freeMachines.back()));
      freeMachines.pop_back();
    }

  // loop over finishing machines and fill them
  while (busyMachines.size() > 0 && std::get<0>(busyMachines.top()) <= toTimestamp)
    {
      timestamp = std::get<0>(busyMachines.top());
      auto& finishedOperation = std::get<2>(busyMachines.top());
      auto& releasedMachineId = std::get<3>(busyMachines.top());
      result.emplace_back(timestamp, finishedOperation, releasedMachineId);
      freeMachines.emplace_front(releasedMachineId, finishedOperation->parentId);
      busyMachines.pop();

      if (queue->size() > 0 && timestamp < toTimestamp)
	{
	  auto operation = queue->pop();
	  // assumption: no operations from future
	  assert(operation->arrival <= timestamp);
	  busyMachines.emplace(timestamp + estimator->estimate(operation),
			       ++assignationsCounter,
			       operation,
			       std::get<0>(freeMachines.back()));
	  freeMachines.pop_back();
	}
    }

  return result;
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
					mFreeMachines,
					mBusyMachines,
					mAssignationsCounter))
    notify(assignment);

  mTimestamp = toTimestamp;
}

std::vector<Assignation> Cloud::simulate(IEstimatorSP estimator,
					 std::vector<OperationSP> operations) const
{
  const auto& fromTimestamp = mTimestamp;
  long long toTimestamp = LLONG_MAX;
  auto queue = std::make_shared<VectorQueue>(operations);
  auto freeMachinesCpy = mFreeMachines;
  auto busyMachinesCpy = mBusyMachines;
  auto assignationsCounterCpy = mAssignationsCounter;
  return process(fromTimestamp,
		 toTimestamp,
		 mMachinesNum,
		 estimator,
		 queue.get(),
		 freeMachinesCpy,
		 busyMachinesCpy,
		 assignationsCounterCpy);
}

std::vector<Assignation> Cloud::simulateWithFuture(IEstimatorSP estimator,
						   std::vector<OperationSP> operations) const
{
  if (operations.size() == 0)
    return {};

  std::vector<Assignation> result;
  std::vector<OperationSP> queue;
  auto queueWrapper = std::make_shared<VectorQueue>(queue);
  auto freeMachinesCpy = mFreeMachines;
  auto busyMachinesCpy = mBusyMachines;
  auto assignationsCounterCpy = mAssignationsCounter;

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
				   freeMachinesCpy,
				   busyMachinesCpy,
				   assignationsCounterCpy);
      result.insert(result.end(), partialResult.begin(), partialResult.end());
      fromTimestamp = toTimestamp;
    }

  return result;
}

void Cloud::assignQueue(IQueue* queue)
{
  mQueue = queue;
}

unsigned Cloud::getMachinesNum()
{
  return mMachinesNum;
}
