#include <algorithm>

#include "SJCapacityScheduler.hpp"
#include "FIFOCapacityScheduler.hpp"

void SJCapacityScheduler::schedule(CapacitySchedule & schedule, JobSP job)
{
  FIFOCapacityScheduler fifo(mInput, mMachines, mEstimator);
  fifo.schedule(schedule, job);

  using JobWeight = long long;
  std::unordered_map<JobID, JobWeight> jobWeights;

  for (const auto& vec : schedule.schedule)
    for (const auto& operation : vec)
    {
      if (jobWeights.find(operation->parentId) == jobWeights.end())
        jobWeights[operation->parentId] = 0;
      jobWeights[operation->parentId] += mEstimator->estimate(operation);
    }

  for (auto& vec : schedule.schedule)
    std::stable_sort(vec.begin(), vec.end(), [&](OperationSP a, OperationSP b) {
        return jobWeights[a->parentId] < jobWeights[b->parentId];  // ASC
      });
}
