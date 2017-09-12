#include <iostream>

#include "Cloud.hpp"

Cloud::Cloud(unsigned machinesNum) :
  mMachinesNum(machinesNum),
  mTimestamp(0),
  mQueue(std::make_shared<std::deque<std::shared_ptr<Operation> > >()),
  mMachines()
{
}

void Cloud::advance(long long toTimestamp)
{
  while (mMachines.size() < mMachinesNum && mQueue->size() > 0)
    {
      auto operation = mQueue->back();
      mQueue->pop_back();

      auto newMachine = Machine{mTimestamp + operation->duration, operation};
      mMachines.push(newMachine);
    }

  while (mMachines.size() > 0 && mMachines.top().first <= toTimestamp)
    {
      mTimestamp = mMachines.top().first;
      mMachines.pop();
      std::cout << mTimestamp << std::endl;

      if (mQueue->size() > 0)
	{
	  auto operation = mQueue->back();
	  mQueue->pop_back();

	  auto newMachine = Machine{mTimestamp + operation->duration, operation};
	  mMachines.push(newMachine);
	}
    }
}

Queue Cloud::getQueue()
{
  return mQueue;
}
