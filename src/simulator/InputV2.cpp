#include <iostream>
#include <cassert>
#include <map>

#include "InputV2.hpp"

void InputV2::readFromStdin()
{
  unsigned zero;
  unsigned version;
  std::cin >> zero;
  std::cin >> version;
  assert(zero == 0);
  assert(version == 2);

  std::map<JobID, JobSP> jobs;
  std::map<JobID, std::vector<OperationSP> > jobOperations;
  unsigned jobsNum;
  std::cin >> jobsNum;
  for (unsigned i = 0; i < jobsNum; i++)
    {
      long long jobId;
      long long jobPriority;
      long long jobArrivalTimestamp;
      std::cin >> jobId;
      std::cin >> jobPriority;
      std::cin >> jobArrivalTimestamp;
      jobs.emplace(std::pair<JobID, JobSP>(jobId,
					   new Job(jobId,
						   jobPriority,
						   jobArrivalTimestamp,
						   {})));
      jobOperations.emplace(std::pair<JobID, std::vector<OperationSP> >{jobId, {}});
    }

  unsigned operationsNum;
  std::cin >> operationsNum;
  for (unsigned i = 0; i < operationsNum; i++)
    {
      long long jobId;
      long long operationId;
      long long operationName;
      long long operationResult;
      long long operationDuration;
      long long operationCapacityReq;
      std::cin >> jobId;
      std::cin >> operationId;
      std::cin >> operationName;
      std::cin >> operationResult;
      std::cin >> operationDuration;
      std::cin >> operationCapacityReq;
      jobOperations[jobId].emplace_back(new Operation(operationId,
						      jobId,
						      operationName,
						      operationResult,
						      jobs[jobId]->arrivalTimestamp,
						      operationDuration));
    }

  for (auto const& kv : jobs)
    {
      const auto& job = kv.second;
      mJobs.emplace_back(new Job(job->id,
				 job->priority,
				 job->arrivalTimestamp,
				 jobOperations[job->id]));
    }
}
