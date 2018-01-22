#include <cassert>

#include "Cloud.hpp"

Cloud::Cloud(unsigned machinesNum) :
  mMachinesNum(machinesNum),
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
      auto execution = std::pair<long long, OperationSP>{mTimestamp, finishedOperation};
      notify(execution);
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

std::vector<std::pair<long long, OperationSP> > Cloud::process(unsigned machinesNum,
							       std::vector<OperationSP> operations)
{
  if (operations.size() == 0)
    return {};

  std::vector<std::pair<long long, OperationSP> > result;
  std::priority_queue<Machine, std::vector<Machine>, std::greater<Machine> > machines;
  long long timestamp = operations.back()->arrival;

  while (machines.size() != 0 || operations.size() != 0)
    {
      while (machines.size() < machinesNum &&
	     operations.size() > 0 &&
	     operations.back()->arrival <= timestamp)
	{
	  auto operation = operations.back();
	  operations.pop_back();
	  auto newMachine = Machine{timestamp + operation->duration, operation};
	  machines.push(newMachine);
	}

      while (machines.size() > 0 && machines.top().first <= timestamp)
	{
	  auto finishedTimestamp = machines.top().first;
	  auto finishedOperation = machines.top().second;
	  result.push_back({finishedTimestamp, finishedOperation});
	  machines.pop();
	}

      if (machines.size() < machinesNum)
	{
	  if (operations.size() > 0)
	    timestamp = std::max(timestamp, operations.back()->arrival);
	  else
	    timestamp = machines.top().first;
	}
      else
	timestamp = machines.top().first;
    }

  return result;
}

unsigned Cloud::getMachinesNum()
{
  return mMachinesNum;
}
