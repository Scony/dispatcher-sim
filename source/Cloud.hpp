#pragma once

#include <memory>
#include <deque>
#include <queue>

#include "Operation.hpp"
#include "IQueue.hpp"
#include "ExecutionsSubject.hpp"

using Machine = std::pair<long long, OperationSP>;

class Cloud : public ExecutionsSubject
{
public:
  static std::vector<std::pair<long long, OperationSP> > simulate(unsigned machinesNum,
								  std::vector<OperationSP> operations);
public:
  Cloud(unsigned machinesNum);

  void advance(long long toTimestamp);
  void assignQueue(IQueue* queue);

  unsigned getMachinesNum();

private:
  const unsigned mMachinesNum;

  long long mTimestamp;
  IQueue* mQueue;
  std::priority_queue<Machine, std::vector<Machine>, std::greater<Machine> > mMachines;
};
