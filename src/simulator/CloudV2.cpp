#include <cassert>
#include <iostream>

#include "CloudV2.hpp"

CloudV2::CloudV2(const std::vector<MachineSP>& machines, unsigned setupTime) :
  Cloud(0,0),
  mSetupTime(setupTime),
  mMachines(machines),
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
