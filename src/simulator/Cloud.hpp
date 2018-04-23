#pragma once

#include <memory>
#include <deque>
#include <queue>

#include "Operation.hpp"
#include "IQueue.hpp"
#include "ICloud.hpp"
#include "IEstimator.hpp"

class Cloud : public ICloud
{
 public:
  using BusyMachine = std::tuple<long long,  // timestamp of finish
				 long long,  // assignationsCounter
				 OperationSP,
				 unsigned>;  // machine ID
  using FreeMachine = std::tuple<unsigned,   // machine ID
				 long long>; // recently processed job ID
  using BusyMachines = std::priority_queue<BusyMachine,
					   std::vector<BusyMachine>,
					   std::greater<BusyMachine> >;
  using FreeMachines = std::deque<FreeMachine>;

 public:
  static std::vector<Assignation> process(const long long& fromTimestamp,
					  const long long& toTimestamp,
					  const unsigned& machinesNum,
					  IEstimatorSP estimator,
					  IQueue* queue,
					  FreeMachines& freeMachines,
					  BusyMachines& busyMachines,
					  long long& assignationsCounter,
					  const unsigned& setupTime = 0);

 public:
  Cloud(unsigned machinesNum, unsigned setupTime = 0);
  ~Cloud();

  void advance(long long toTimestamp) override;
  std::vector<Assignation> simulate(IEstimatorSP estimator,
				    std::vector<OperationSP> operations) const override;
  std::vector<Assignation> simulateWithFuture(IEstimatorSP estimator,
					      std::vector<OperationSP> operations) const override;

 private:
  const unsigned mMachinesNum;
  const unsigned mSetupTime;

  long long mTimestamp;
  FreeMachines mFreeMachines;
  BusyMachines mBusyMachines;
  long long mAssignationsCounter;
};
