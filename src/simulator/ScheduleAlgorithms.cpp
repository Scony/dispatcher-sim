#include <cassert>
#include <algorithm>

#include "ScheduleAlgorithms.hpp"

void ScheduleAlgorithms::fifo(Schedule & schedule, JobSP job)
{
  const unsigned machinesNum = schedule.schedule.size();
  assert(machinesNum > 0);

  unsigned machineId = 0;
  for (auto& operation : job->operations)
  {
    schedule.schedule[machineId].push_back(operation);
    machineId = (machineId + 1) % machinesNum;
  }
}

void ScheduleAlgorithms::shortest_job(Schedule & schedule, JobSP job) // TODO: estimator
{
  fifo(schedule, job);

  using JobWeight = long long;
  std::unordered_map<JobID, JobWeight> jobWeights;

  for (const auto& vec : schedule.schedule)
    for (const auto& operation : vec)
    {
      if (jobWeights.find(operation->parentId) == jobWeights.end())
        jobWeights[operation->parentId] = 0;
      jobWeights[operation->parentId] += operation->duration;
    }

  for (auto& vec : schedule.schedule)
    std::sort(vec.begin(), vec.end(), [&](OperationSP& a, OperationSP& b) {
        return jobWeights[a->parentId] < jobWeights[b->parentId];  // ASC
      });
}
