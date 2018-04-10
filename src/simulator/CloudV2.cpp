#include <cassert>
#include <iostream>

#include "CloudV2.hpp"

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
	  freeMachinesMap[releasedBusyMachine.machine->capacity]
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
  mMachines(machines)
{
}

CloudV2::~CloudV2()
{
}

void CloudV2::advance(long long toTimestamp)
{
  assert(mQueue != nullptr);
  assert(mMachines.size() > 0);
  // ...
}

std::vector<Assignation> CloudV2::simulate(IEstimatorSP estimator,
					   std::vector<OperationSP> operations) const
{
  assert(mMachines.size() > 0);
  // ...
  return {};
}

std::vector<Assignation> CloudV2::simulateWithFuture(IEstimatorSP estimator,
						     std::vector<OperationSP> operations) const
{
  assert(mMachines.size() > 0);
  // ...
  return {};
}
