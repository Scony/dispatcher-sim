#include <algorithm>
#include <cassert>

#include "VRDispatcher.hpp"

VRDispatcher::VRDispatcher(std::shared_ptr<Input> input,
			   std::shared_ptr<ICloud> cloud,
			   std::shared_ptr<IEstimator> estimator,
			   bool deterministic) :
  Dispatcher(input, cloud, estimator),
  mDeterministic(deterministic),
  mNextJob(0),
  mNextRule(0)
{
}

void VRDispatcher::dispatch(std::shared_ptr<Job> job)
{
  // append new operations to map
  mJobOperations[job->id] = {};
  mJobOperations[job->id].insert(mJobOperations[job->id].end(),
				 job->operations.begin(),
				 job->operations.end());
  std::sort(mJobOperations[job->id].begin(),
	    mJobOperations[job->id].end(),
	    [&](OperationSP a, OperationSP b) {
	      return mEstimator->estimate(a) < mEstimator->estimate(b); // ASC
	    });

  // update weights of jobs
  mJobsInOrder.clear();
  for (const auto& kv : mJobOperations)
    {
      const auto& jobId = kv.first;
      const auto& remainingJobOperations = kv.second;

      long long jobWeight = 0;
      for (const auto& operation : remainingJobOperations)
	jobWeight += mEstimator->estimate(operation);

      mJobsInOrder.push_back({jobWeight, jobId});
    }
  std::sort(mJobsInOrder.begin(), mJobsInOrder.end(), std::greater<std::pair<long long, long long> >());
}

OperationSP VRDispatcher::peek()
{
  assert(false);		// FIXME
  return OperationSP(nullptr);
}

OperationSP VRDispatcher::pop()
{
  assert(mJobOperations.size() > 0);

  nextRule();

  switch (mNextRule)
    {
    case 0:			// SJLO
      {
	const auto& jobToPopFrom = mJobsInOrder.back().second;

	auto operation = mJobOperations[jobToPopFrom].back();
	mJobOperations[jobToPopFrom].pop_back();

	if (mJobOperations[jobToPopFrom].size() == 0)
	  {
	    mJobOperations.erase(jobToPopFrom);
	    mJobsInOrder.pop_back();
	  }

	if (mJobOperations.size() == 0)
	  mNextJob = 0;
	else
	  mNextJob = mNextJob % mJobOperations.size();

	return operation;
      }
    case 1:			// SJSO
      {
	const auto& jobToPopFrom = mJobsInOrder.back().second;

	auto operation = mJobOperations[jobToPopFrom].front();
	mJobOperations[jobToPopFrom].pop_front();

	if (mJobOperations[jobToPopFrom].size() == 0)
	  {
	    mJobOperations.erase(jobToPopFrom);
	    mJobsInOrder.pop_back();
	  }

	if (mJobOperations.size() == 0)
	  mNextJob = 0;
	else
	  mNextJob = mNextJob % mJobOperations.size();

	return operation;
      }
    case 2:			// MAX
      {
	auto jobToPopFrom = mJobOperations.begin();
	std::advance(jobToPopFrom, mNextJob);

	auto operation = jobToPopFrom->second.back();
	jobToPopFrom->second.pop_back();

	if (jobToPopFrom->second.size() == 0)
	  {
	    auto jobId = jobToPopFrom->first;
	    for (auto it = mJobsInOrder.begin(); it != mJobsInOrder.end(); it++)
	      if (it->second == jobId)
		{
		  mJobsInOrder.erase(it);
		  break;
		}
	    mJobOperations.erase(jobToPopFrom);
	  }

	if (mJobOperations.size() == 0)
	  mNextJob = 0;
	else
	  mNextJob = (mNextJob + 1) % mJobOperations.size();

	return operation;
      }
    case 3:			// MIN
      {
	auto jobToPopFrom = mJobOperations.begin();
	std::advance(jobToPopFrom, mNextJob);

	auto operation = jobToPopFrom->second.front();
	jobToPopFrom->second.pop_front();

	if (jobToPopFrom->second.size() == 0)
	  {
	    auto jobId = jobToPopFrom->first;
	    for (auto it = mJobsInOrder.begin(); it != mJobsInOrder.end(); it++)
	      if (it->second == jobId)
		{
		  mJobsInOrder.erase(it);
		  break;
		}
	    mJobOperations.erase(jobToPopFrom);
	  }

	if (mJobOperations.size() == 0)
	  mNextJob = 0;
	else
	  mNextJob = (mNextJob + 1) % mJobOperations.size();

	return operation;
      }
    }

  assert(false);
  return OperationSP(nullptr);
}

size_t VRDispatcher::size()
{
  size_t totalSize = 0;
  for (const auto& kv : mJobOperations)
    totalSize += kv.second.size();

  return totalSize;
}

void VRDispatcher::nextRule()
{
  if (mDeterministic)
    mNextRule = (mNextRule + 1) % mRules;
  else
    mNextRule = rand() % mRules;
}
