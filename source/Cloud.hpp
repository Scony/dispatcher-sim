#pragma once

#include <memory>
#include <deque>
#include <queue>

#include "Operation.hpp"
#include "Solution.hpp"

using Queue = std::shared_ptr<std::deque<std::shared_ptr<Operation> > >;
using Machine = std::pair<long long, std::shared_ptr<Operation> >;

class Cloud
{
public:
  Cloud(unsigned machinesNum, std::shared_ptr<Solution::Solution> solution);

  void advance(long long toTimestamp);
  Queue getQueue();

private:
  const unsigned mMachinesNum;

  std::shared_ptr<Solution::Solution> mSolution;
  long long mTimestamp;
  Queue mQueue;
  std::priority_queue<Machine, std::vector<Machine>, std::greater<Machine> > mMachines;
};
