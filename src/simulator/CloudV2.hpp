#pragma once

#include "Cloud.hpp"
#include "Machine.hpp"

class CloudV2 : public Cloud
{
public:
  CloudV2(unsigned setupTime = 0);
  ~CloudV2();

  void advance(long long toTimestamp);
  std::vector<Assignation> simulate(IEstimatorSP estimator,
				    std::vector<OperationSP> operations) const override;
  std::vector<Assignation> simulateWithFuture(IEstimatorSP estimator,
					      std::vector<OperationSP> operations) const override;
  void assignQueue(IQueue* queue);

  void readMachinesFromStdin();

private:
  const unsigned mSetupTime;

  std::vector<MachineSP> mMachines;

  IQueue* mQueue;
};
