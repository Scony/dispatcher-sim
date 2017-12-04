#include <algorithm>
#include <map>
#include <cassert>

#include "MaxDispatcher.hpp"

MaxDispatcher::MaxDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud) :
  Dispatcher(input, cloud),
  mJobOperations(),
  mNextJob(0)
{
}

void MaxDispatcher::dispatch(JobSP job)
{
  mJobOperations[job->id] = job->operations;
  std::sort(mJobOperations[job->id].begin(),
	    mJobOperations[job->id].end(),
	    [](OperationSP a, OperationSP b) {
	      return a->duration < b->duration; // ASC // TODO: estimation
	    });
}

OperationSP MaxDispatcher::pop()
{
  assert(size() > 0);

  auto jobToPopFrom = mJobOperations.begin();
  std::advance(jobToPopFrom, mNextJob);

  auto operation = jobToPopFrom->second.back();
  jobToPopFrom->second.pop_back();

  if (jobToPopFrom->second.size() == 0)
    mJobOperations.erase(jobToPopFrom);

  if (mJobOperations.size() == 0)
    mNextJob = 0;
  else
    mNextJob = (mNextJob + 1) % mJobOperations.size();

  return operation;
}

size_t MaxDispatcher::size()
{
  size_t totalSize = 0;
  for (const auto& kv : mJobOperations)
    totalSize += kv.second.size();

  return totalSize;
}
