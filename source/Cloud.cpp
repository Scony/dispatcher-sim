#include <cassert>

#include "Cloud.hpp"

Cloud::Cloud(unsigned machinesNum, std::shared_ptr<Solution::Solution> solution) :
  mMachinesNum(machinesNum),
  mSolution(solution),
  mTimestamp(0),
  mQueue(nullptr),
  mMachines()
{
}

void Cloud::advance(long long toTimestamp)
{
  assert(mQueue != nullptr);

  while (mMachines.size() < mMachinesNum && mQueue->size() > 0)
    {
      auto operation = mQueue->pop();
      auto newMachine = Machine{mTimestamp + operation->duration, operation};
      mMachines.push(newMachine);
    }

  while (mMachines.size() > 0 && mMachines.top().first <= toTimestamp)
    {
      mTimestamp = mMachines.top().first;
      auto finishedOperation = mMachines.top().second;
      mSolution->push_back({mTimestamp, finishedOperation});
      mMachines.pop();

      if (mQueue->size() > 0)
	{
	  auto operation = mQueue->pop();
	  auto newMachine = Machine{mTimestamp + operation->duration, operation};
	  mMachines.push(newMachine);
	}
    }

  mTimestamp = toTimestamp;
}

void Cloud::assignQueue(IQueue* queue)
{
  mQueue = queue;
}
