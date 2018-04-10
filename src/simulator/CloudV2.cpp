#include <cassert>
#include <iostream>
#include <unordered_set>
#include <climits>

#include "CloudV2.hpp"
#include "VectorQueue.hpp"
#include "NoEstimator.hpp"

CloudV2::FreeMachine::FreeMachine(MachineSP aMachine, long long aRecentJobId) :
  machine(aMachine),
  recentJobId(aRecentJobId)
{
}

CloudV2::BusyMachine::BusyMachine(MachineSP aMachine,
				  OperationSP aOperation,
				  long long aFinishTimestamp,
				  long long aLogicalClock) :
  machine(aMachine),
  operation(aOperation),
  finishTimestamp(aFinishTimestamp),
  logicalClock(aLogicalClock)
{
}

bool CloudV2::BusyMachine::operator<(const BusyMachine& other) const
{
  if (finishTimestamp != other.finishTimestamp)
    return finishTimestamp < other.finishTimestamp;
  if (logicalClock != other.logicalClock)
    return logicalClock < other.logicalClock;
  return operation->id < other.operation->id;
}

bool CloudV2::BusyMachine::operator>(const BusyMachine& other) const
{
  return other < *this;
}

bool CloudV2::BusyMachine::operator==(const BusyMachine& other) const
{
  return machine == other.machine &&
    operation == other.operation &&
    finishTimestamp == other.finishTimestamp &&
    logicalClock == other.logicalClock;
}

std::vector<Assignation> CloudV2::process(const long long& fromTimestamp,
					  const long long& toTimestamp,
					  IEstimatorSP estimator,
					  IQueue* queue,
					  FreeMachines& freeMachinesMap,
					  BusyMachines& busyMachines,
					  long long& assignationsCounter,
					  const unsigned& setupTime)
{
  // assumption: machines finishing before or at fromTimestamp should be emptied
  assert(busyMachines.size() == 0 || busyMachines.top().finishTimestamp > fromTimestamp);

  std::vector<Assignation> result;
  long long timestamp = fromTimestamp;

  while (true)
    {
      // merge machine pieces
      using MachineId = long long;
      std::unordered_set<MachineId> machinesFreeOnce;
      std::unordered_set<MachineId> machinesFreeMoreThanOnce;

      for (const auto& kv : freeMachinesMap)
	for (const auto& freeMachine : kv.second)
	  if (machinesFreeOnce.find(freeMachine.machine->id) == machinesFreeOnce.end())
	    machinesFreeOnce.insert(freeMachine.machine->id);
	  else
	    machinesFreeMoreThanOnce.insert(freeMachine.machine->id);

      for (const auto& machineId : machinesFreeMoreThanOnce)
	{
	  long long mergedCapacity = 0;
	  long long recentJobIdFromBiggestPart = -1;
	  MachineSP machine(nullptr);

	  for (auto& kv : freeMachinesMap)
	    {
	      const auto& capacity = kv.first;
	      auto& freeMachines = kv.second;
	      for (auto it = freeMachines.begin(); it != freeMachines.end();)
		if (it->machine->id == machineId)
		  {
		    mergedCapacity += capacity;
		    recentJobIdFromBiggestPart = it->recentJobId;
		    machine = it->machine;
		    it = freeMachines.erase(it);
		  }
		else
		  it++;
	    }

	  if (freeMachinesMap.find(mergedCapacity) == freeMachinesMap.end())
	    freeMachinesMap[mergedCapacity] = {};
	  freeMachinesMap[mergedCapacity].emplace_front(machine, recentJobIdFromBiggestPart);
	}

      // fill free machines
      while (queue->size() > 0)
	{
	  auto operation = queue->peek();
	  // assumption: no operations from future
	  assert(operation->arrival <= timestamp);
	  auto it = freeMachinesMap.lower_bound(operation->capacityReq);
	  // assumption at least one machine class satisfies capacityReq
	  assert(it != freeMachinesMap.end());
	  while (it != freeMachinesMap.end() && it->second.size() == 0)
	    it++;
	  if (it == freeMachinesMap.end()) // no free machines given capacityReq
	    break;
	  else
	    {
	      auto& freeMachinesQueue = it->second;
	      const auto& freeMachine = freeMachinesQueue.back();
	      busyMachines.emplace(freeMachine.machine,
				   operation,
				   timestamp +
				   estimator->estimate(operation) +
				   (freeMachine.recentJobId == operation->parentId ? 0 : setupTime),
				   ++assignationsCounter);
	      freeMachinesQueue.pop_back();
	      queue->pop();

	      // remaining capacity reusing
	      long long remainingCapacity = freeMachine.machine->capacity - operation->capacityReq;
	      if (remainingCapacity > 0)
		{
		  if (freeMachinesMap.find(remainingCapacity) == freeMachinesMap.end())
		    freeMachinesMap[remainingCapacity] = {};
		  freeMachinesMap[remainingCapacity].emplace_front(freeMachine.machine,
								   freeMachine.recentJobId);
		}
	    }
	}

      // stop if nothing more to do in this timeframe
      if (busyMachines.size() == 0 || busyMachines.top().finishTimestamp > toTimestamp)
	break;

      // jump to next finishing machine
      timestamp = busyMachines.top().finishTimestamp;

      // free all machines at that point
      while (busyMachines.size() > 0 && busyMachines.top().finishTimestamp == timestamp)
	{
	  const auto& releasedBusyMachine = busyMachines.top();
	  if (freeMachinesMap.find(releasedBusyMachine.operation->capacityReq) == freeMachinesMap.end())
	    freeMachinesMap[releasedBusyMachine.operation->capacityReq] = {};
	  freeMachinesMap[releasedBusyMachine.operation->capacityReq]
	    .emplace_front(releasedBusyMachine.machine,
			   releasedBusyMachine.operation->parentId);
	  result.emplace_back(timestamp,
			      releasedBusyMachine.operation,
			      releasedBusyMachine.machine->id);
	  busyMachines.pop();
	}
    }

  return result;
}

CloudV2::CloudV2(const std::vector<MachineSP>& machines, unsigned setupTime) :
  mSetupTime(setupTime),
  mTimestamp(0),
  mAssignationsCounter(0)
{
  assert(machines.size() > 0);

  for (auto& machine : machines)
    {
      if (mFreeMachines.find(machine->capacity) == mFreeMachines.end())
	mFreeMachines[machine->capacity] = {};
      mFreeMachines[machine->capacity].emplace_front(machine, -1);
    }
}

CloudV2::~CloudV2()
{
}

void CloudV2::advance(long long toTimestamp)
{
  assert(mQueue != nullptr);

  static IEstimatorSP noEstimator(new NoEstimator);

  for (auto const& assignment : process(mTimestamp,
					toTimestamp,
					noEstimator,
					mQueue,
					mFreeMachines,
					mBusyMachines,
					mAssignationsCounter,
					mSetupTime))
    notify(assignment);

  mTimestamp = toTimestamp;
}

std::vector<Assignation> CloudV2::simulate(IEstimatorSP estimator,
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
		 estimator,
		 queue.get(),
		 freeMachinesCpy,
		 busyMachinesCpy,
		 assignationsCounterCpy,
		 mSetupTime);
}

std::vector<Assignation> CloudV2::simulateWithFuture(IEstimatorSP estimator,
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
				   estimator,
				   queueWrapper.get(),
				   freeMachinesCpy,
				   busyMachinesCpy,
				   assignationsCounterCpy,
				   mSetupTime);
      result.insert(result.end(), partialResult.begin(), partialResult.end());
      fromTimestamp = toTimestamp;
    }

  return result;
}
