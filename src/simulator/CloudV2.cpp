#include <cassert>
#include <iostream>

#include "CloudV2.hpp"

CloudV2::CloudV2(unsigned setupTime) :
  Cloud(0,0),
  mSetupTime(setupTime),
  mQueue(nullptr)
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

void CloudV2::assignQueue(IQueue* queue)
{
  mQueue = queue;
}

void CloudV2::readMachinesFromStdin()
{
  unsigned version;
  std::cin >> version;
  assert(version == 1);

  unsigned machinesNum;
  std::cin >> machinesNum;
  assert(machinesNum > 0);

  for (unsigned i = 0; i < machinesNum; i++)
    {
      long long machineId;
      long long machineCapacity;
      std::cin >> machineId;
      std::cin >> machineCapacity;
      mMachines.emplace_back(new Machine(machineId, machineCapacity));
    }
}
