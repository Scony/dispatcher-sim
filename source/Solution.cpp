#include <map>
#include <cassert>
#include <algorithm>

#include "Solution.hpp"

namespace Solution
{
  void validate(std::vector<std::shared_ptr<Job> > jobs, std::shared_ptr<Solution> solution, unsigned machinesNum)
  {
    std::map<long long, std::shared_ptr<Job> > jobsMap;
    for (const auto& job : jobs)
      jobsMap[job->id] = job;

    // each operation ended properly
    for (const auto& pair : *solution)
      {
	const auto& endTimestamp = pair.first;
	const auto& operation = pair.second;

	assert(endTimestamp - operation->duration >= jobsMap[operation->parentId]->arrivalTimestamp);
      }

    // each operation executed exactly once
    std::map<long long, unsigned> operationExecutions;
    for (const auto& job : jobs)
      for (const auto& operation : job->operations)
	operationExecutions[operation->id] = 0;
    for (const auto& pair : *solution)
      {
	const auto& operation = pair.second;
	operationExecutions[operation->id]++;
      }
    for (const auto& kv : operationExecutions)
      {
	const auto& executionsNum = kv.second;
	assert(executionsNum == 1);
      }
    
    // solution is schedulable given the number of machines
    // FIXME: make sure this algorithm is correct
    std::vector<std::pair<long long, int> > cloudUsageChanges;
    for (const auto& pair : *solution)
      {
	const auto& endTimestamp = pair.first;
	const auto& operation = pair.second;
	long long beginTimestamp = endTimestamp - operation->duration;
	cloudUsageChanges.push_back({beginTimestamp, +1});
	cloudUsageChanges.push_back({endTimestamp, -1});
      }
    std::sort(cloudUsageChanges.begin(), cloudUsageChanges.end());
    int machinesInUse = 0;
    for (const auto& kv : cloudUsageChanges)
      {
	const auto& usageChange = kv.second;
	machinesInUse += usageChange;
	assert(machinesInUse <= (int)machinesNum);
      }

    std::sort(cloudUsageChanges.begin(), cloudUsageChanges.end(), [](std::pair<long long, int> a,
								     std::pair<long long, int> b) {
	if (a.first != b.first)
	  return a < b;
	return a.second > b.second;
      });
    machinesInUse = 0;
    for (const auto& kv : cloudUsageChanges)
      {
	const auto& usageChange = kv.second;
	machinesInUse += usageChange;
	assert(machinesInUse >= 0);
      }
  }

  std::vector<std::pair<long long, std::shared_ptr<Job> > > calculateFlow(std::vector<std::shared_ptr<Job> > jobs,
									  std::shared_ptr<Solution> solution)
  {
    std::map<long long, long long> jobEnds;

    for (const auto& pair : *solution)
      {
	const auto& endTimestamp = pair.first;
	const auto& operation = pair.second;

	if (jobEnds.find(operation->parentId) == jobEnds.end())
	  jobEnds[operation->parentId] = 0;

	jobEnds[operation->parentId] = std::max(jobEnds[operation->parentId], endTimestamp);
      }

    std::vector<std::pair<long long, std::shared_ptr<Job> > > jobFlows;

    for (auto& job : jobs)
      jobFlows.push_back({jobEnds[job->id] - job->arrivalTimestamp, job});

    return jobFlows;
  }
}
