#pragma once

#include "Cloud.hpp"
#include "Machine.hpp"

class CloudV2 : public Cloud
{
public:
  CloudV2(const std::vector<MachineSP>& machines, unsigned setupTime = 0);
  ~CloudV2();

  void advance(long long toTimestamp);
  std::vector<Assignation> simulate(IEstimatorSP estimator,
				    std::vector<OperationSP> operations) const override;
  std::vector<Assignation> simulateWithFuture(IEstimatorSP estimator,
					      std::vector<OperationSP> operations) const override;
  void assignQueue(IQueue* queue);

private:
  const unsigned mSetupTime;
  const std::vector<MachineSP>& mMachines;

  IQueue* mQueue;
};
