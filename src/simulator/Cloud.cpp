#include <cassert>
#include <climits>

#include "Cloud.hpp"
#include "NoEstimator.hpp"
#include "VectorQueue.hpp"

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
  static IEstimatorSP noEstimator(new NoEstimator);

  for (auto const& assignment : process(mTimestamp,
					toTimestamp,
					mMachinesNum,
					noEstimator,
					mQueue,
					mMachines))
    notify(assignment);

  mTimestamp = toTimestamp;
}

void Cloud::assignQueue(IQueue* queue)
{
  mQueue = queue;
}

std::vector<std::pair<long long, OperationSP> > Cloud::process(const long long& fromTimestamp,
							       const long long& toTimestamp,
							       const unsigned& machinesNum,
							       IEstimatorSP estimator,
							       IQueue* queue,
							       Machines& machines)
{
  // assumption: machines finishing before or at fromTimestamp should be emptied
  assert(machines.size() == 0 || machines.top().first > fromTimestamp);

  std::vector<std::pair<long long, OperationSP> > result;
  long long timestamp = fromTimestamp;

  // fill machines
  while (machines.size() < machinesNum && queue->size() > 0)
    {
      auto operation = queue->pop();
      // assumption: no operations from future
      assert(operation->arrival <= timestamp);
      machines.emplace(timestamp + estimator->estimate(operation), operation);
    }

  // loop over finishing machines and fill them
  while (machines.size() > 0 && machines.top().first <= toTimestamp)
    {
      timestamp = machines.top().first;
      auto& finishedOperation = machines.top().second;
      result.emplace_back(timestamp, finishedOperation);
      machines.pop();

      if (queue->size() > 0 && timestamp < toTimestamp)
	{
	  auto operation = queue->pop();
	  // assumption: no operations from future
	  assert(operation->arrival <= timestamp);
	  machines.emplace(timestamp + estimator->estimate(operation), operation);
	}
    }

  return result;
}

std::vector<std::pair<long long, OperationSP> > Cloud::simulate(unsigned machinesNum,
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

std::vector<std::pair<long long, OperationSP> > Cloud::simulate(IEstimatorSP estimator,
								std::vector<OperationSP> operations)
  const
{
  const auto& fromTimestamp = mTimestamp;
  long long toTimestamp = LLONG_MAX;
  auto queue = std::make_shared<VectorQueue>(operations);
  auto machinesCpy = mMachines;
  return process(fromTimestamp,
		 toTimestamp,
		 mMachinesNum,
		 estimator,
		 queue.get(),
		 machinesCpy);
}

unsigned Cloud::getMachinesNum()
{
  return mMachinesNum;
}
