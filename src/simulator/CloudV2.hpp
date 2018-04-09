#pragma once

#include <deque>
#include <queue>
#include <map>

#include "ICloud.hpp"
#include "Machine.hpp"

class CloudV2 : public ICloud
{
public:
  struct FreeMachine
  {
    FreeMachine(MachineSP aMachine, long long aRecentJobId = -1);

    MachineSP machine;
    long long recentJobId;
  };
  struct BusyMachine
  {
    BusyMachine(MachineSP aMachine,
		OperationSP aOperation,
		long long aFinishTimestamp,
		long long aLogicalClock);

    bool operator<(const BusyMachine& other) const;
    bool operator>(const BusyMachine& other) const;
    bool operator==(const BusyMachine& other) const;

    MachineSP machine;
    OperationSP operation;
    long long finishTimestamp;
    long long logicalClock;
  };
  using FreeMachines = std::map<long long,
				std::deque<FreeMachine> >;
  using BusyMachines = std::priority_queue<BusyMachine,
					   std::vector<BusyMachine>,
					   std::greater<BusyMachine> >;

public:
  static std::vector<Assignation> process(const long long& fromTimestamp,
					  const long long& toTimestamp,
					  IEstimatorSP estimator,
					  IQueue* queue,
					  FreeMachines& freeMachines,
					  BusyMachines& busyMachines,
					  long long& assignationsCounter,
					  const unsigned& setupTime);

public:
  CloudV2(const std::vector<MachineSP>& machines, unsigned setupTime = 0);
  ~CloudV2();

  void advance(long long toTimestamp) override;
  std::vector<Assignation> simulate(IEstimatorSP estimator,
				    std::vector<OperationSP> operations) const override;
  std::vector<Assignation> simulateWithFuture(IEstimatorSP estimator,
					      std::vector<OperationSP> operations) const override;

private:
  const unsigned mSetupTime;
  const std::vector<MachineSP>& mMachines;
};
