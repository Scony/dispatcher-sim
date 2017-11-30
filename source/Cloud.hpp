#pragma once

#include <memory>
#include <deque>
#include <queue>

#include "Operation.hpp"
#include "Solution.hpp"
#include "IQueue.hpp"

using Machine = std::pair<long long, OperationSP>;

class Cloud
{
public:
  Cloud(unsigned machinesNum, std::shared_ptr<Solution::Solution> solution);

  void advance(long long toTimestamp);
  void assignQueue(IQueue* queue);

private:
  const unsigned mMachinesNum;

  std::shared_ptr<Solution::Solution> mSolution;
  long long mTimestamp;
  IQueue* mQueue;
  std::priority_queue<Machine, std::vector<Machine>, std::greater<Machine> > mMachines;
};
