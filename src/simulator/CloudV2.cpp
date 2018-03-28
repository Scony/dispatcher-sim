#include <cassert>

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
  // ...
}

std::vector<Assignation> CloudV2::simulate(IEstimatorSP estimator,
					   std::vector<OperationSP> operations) const
{
  // ...
  return {};
}

std::vector<Assignation> CloudV2::simulateWithFuture(IEstimatorSP estimator,
						     std::vector<OperationSP> operations) const
{
  // ...
  return {};
}

void CloudV2::assignQueue(IQueue* queue)
{
  mQueue = queue;
}
