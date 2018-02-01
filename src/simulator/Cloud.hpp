#pragma once

#include <memory>
#include <deque>
#include <queue>

#include "Operation.hpp"
#include "IQueue.hpp"
#include "ExecutionsSubject.hpp"
#include "IEstimator.hpp"

using Machine = std::pair<long long, OperationSP>;

class Cloud : public ExecutionsSubject
{
public:
  using Machines = std::priority_queue<Machine, std::vector<Machine>, std::greater<Machine> >;

public:
  static std::vector<Assignation> process(const long long& fromTimestamp,
					  const long long& toTimestamp,
					  const unsigned& machinesNum,
					  IEstimatorSP estimator,
					  IQueue* queue,
					  Machines& machines);

public:
  Cloud(unsigned machinesNum);

  void advance(long long toTimestamp);
  std::vector<Assignation> simulate(IEstimatorSP estimator,
				    std::vector<OperationSP> operations) const;
  std::vector<Assignation> simulateWithFuture(IEstimatorSP estimator,
					      std::vector<OperationSP> operations) const;
  void assignQueue(IQueue* queue);

  unsigned getMachinesNum();

private:
  const unsigned mMachinesNum;

  long long mTimestamp;
  IQueue* mQueue;
  std::priority_queue<Machine, std::vector<Machine>, std::greater<Machine> > mMachines;
};
