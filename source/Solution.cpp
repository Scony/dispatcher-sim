#include <map>
#include <cassert>

#include "Solution.hpp"

namespace Solution
{
  void validate(std::vector<std::shared_ptr<Job> > jobs, std::shared_ptr<Solution> solution)
  {
    std::map<long long, std::shared_ptr<Job> > jobsMap;
    for (auto& job : jobs)
      jobsMap[job->id] = job;

    for (const auto& pair : *solution)
      {
	const auto& endTimestamp = pair.first;
	const auto& operation = pair.second;

	assert(endTimestamp - operation->duration >= jobsMap[operation->parentId]->arrivalTimestamp);
      }

    // TODO: check if each operation executed exactly once
    // TODO: check if solution is schedulable given the number of machines
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
