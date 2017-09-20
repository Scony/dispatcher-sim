#pragma once

#include <memory>
#include <deque>
#include <queue>

#include "Operation.hpp"
#include "Solution.hpp"

using Queue = std::deque<OperationSP>;
using QueueSP = std::shared_ptr<Queue>;
using Machine = std::pair<long long, OperationSP>;

class Cloud
{
public:
  Cloud(unsigned machinesNum, std::shared_ptr<Solution::Solution> solution);

  void advance(long long toTimestamp);
  QueueSP getQueue();

private:
  const unsigned mMachinesNum;

  std::shared_ptr<Solution::Solution> mSolution;
  long long mTimestamp;
  QueueSP mQueue;
  std::priority_queue<Machine, std::vector<Machine>, std::greater<Machine> > mMachines;
};
