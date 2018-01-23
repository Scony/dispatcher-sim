#include <cassert>
#include <algorithm>

#include "JOPTDispatcher.hpp"

JOPTDispatcher::JOPTDispatcher(std::shared_ptr<Input> input,
			       std::shared_ptr<Cloud> cloud,
			       std::shared_ptr<IEstimator> estimator,
			       std::string operationLevelAlgorithm) :
  QOPTDispatcher(input, cloud, estimator, false)
{
  std::vector<JobSP> jobPermutation;
  std::map<long long, std::vector<OperationSP> > jobOperations;
  for (auto const& job : input->getJobs())
    {
      jobPermutation.push_back(job);
      jobOperations[job->id] = {};
      jobOperations[job->id].insert(jobOperations[job->id].end(),
				    job->operations.begin(),
				    job->operations.end());

      if (operationLevelAlgorithm == "random")
	std::random_shuffle(jobOperations[job->id].begin(), jobOperations[job->id].end());
      else if (operationLevelAlgorithm == "max")
	std::sort(jobOperations[job->id].begin(),
		  jobOperations[job->id].end(),
		  [&](OperationSP a, OperationSP b) {
		    return a->duration < b->duration; // ASC
		  });
      else if (operationLevelAlgorithm == "min")
	std::sort(jobOperations[job->id].begin(),
		  jobOperations[job->id].end(),
		  [&](OperationSP a, OperationSP b) {
		    return a->duration > b->duration; // DESC
		  });
      else
	assert(false);
    }

  std::sort(jobPermutation.begin(), jobPermutation.end());
  std::vector<OperationSP> opPermutation;
  for (auto const& job : jobPermutation)
    opPermutation.insert(opPermutation.end(),
			 jobOperations[job->id].begin(),
			 jobOperations[job->id].end());

  auto bestJobPermutation = jobPermutation;
  auto bestOpPermutation = opPermutation;

  // TODO

  mQueue = bestOpPermutation;
}
