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
