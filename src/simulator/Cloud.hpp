#pragma once

#include <memory>
#include <deque>
#include <queue>

#ifdef USE_HEAP_FROM_BOOST
#include <boost/heap/skew_heap.hpp>
#endif

#include "Operation.hpp"
#include "IQueue.hpp"
#include "ExecutionsSubject.hpp"
#include "IEstimator.hpp"

using BusyMachine = std::tuple<long long,  // timestamp of finish
			       long long,  // assignationsCounter
			       OperationSP,
			       unsigned>;  // machine ID
using FreeMachine = std::tuple<unsigned,   // machine ID
			       long long>; // recently processed job ID

class Cloud : public ExecutionsSubject
{
public:
#ifndef USE_HEAP_FROM_BOOST
  using BusyMachines = std::priority_queue<BusyMachine,
					   std::vector<BusyMachine>,
					   std::greater<BusyMachine> >;
#else
  using BusyMachines = boost::heap::skew_heap<BusyMachine,
					      boost::heap::compare<std::greater<BusyMachine> > >;
#endif
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

  void advance(long long toTimestamp);
  std::vector<Assignation> simulate(IEstimatorSP estimator,
				    std::vector<OperationSP> operations) const;
  std::vector<Assignation> simulateWithFuture(IEstimatorSP estimator,
					      std::vector<OperationSP> operations) const;
  void assignQueue(IQueue* queue);

  unsigned getMachinesNum();

private:
  const unsigned mMachinesNum;
  const unsigned mSetupTime;

  long long mTimestamp;
  IQueue* mQueue;
  FreeMachines mFreeMachines;
  BusyMachines mBusyMachines;
  long long mAssignationsCounter;
};
