#include <algorithm>

#include "MinDispatcher.hpp"

MinDispatcher::MinDispatcher(std::shared_ptr<Input> input,
			     std::shared_ptr<Cloud> cloud,
			     std::shared_ptr<IEstimator> estimator) :
  MaxDispatcher(input, cloud, estimator)
{
}

void MinDispatcher::dispatch(JobSP job)
{
  mJobOperations[job->id] = job->operations;
  std::sort(mJobOperations[job->id].begin(),
	    mJobOperations[job->id].end(),
	    [](OperationSP a, OperationSP b) {
	      return a->duration > b->duration; // DESC // TODO: estimation
	    });
}
