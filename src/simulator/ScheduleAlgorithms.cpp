#include <cassert>
#include <algorithm>
#include <queue>

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

void ScheduleAlgorithms::shortest_job_longest_operation(Schedule & schedule, JobSP job)
{  // TODO: estimator
  const unsigned machinesNum = schedule.schedule.size();

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

  std::vector<OperationSP> operations;
  for (auto& vec : schedule.schedule)
  {
    operations.insert(operations.end(), vec.begin(), vec.end());
    vec.clear();
  }
  std::sort(operations.begin(), operations.end(), [&](OperationSP& a, OperationSP& b) {
      if (a->parentId != b->parentId)
        return jobWeights[a->parentId] > jobWeights[b->parentId];  // SJ
      else
        return a->duration < b->duration;  // LO
    });

  using ReadyTime = long long;
  using MachineID = unsigned;
  using Pair = std::pair<ReadyTime, MachineID>;
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair> > leastBusyMachines;
  for (unsigned machine = 0; machine < machinesNum; machine++)
  {
    if (schedule.ongoings.find(machine) == schedule.ongoings.end())
      leastBusyMachines.push({0, machine});
    else
    {
      long long ongoingFinish = schedule.ongoings[machine].first +
          schedule.ongoings[machine].second->duration;
      leastBusyMachines.push({ongoingFinish - job->arrivalTimestamp, machine});
    }
  }

  while (operations.size() > 0)
  {
    ReadyTime leastBusyMachineReadyTime = leastBusyMachines.top().first;
    MachineID leastBusyMachine = leastBusyMachines.top().second;
    leastBusyMachines.pop();
    schedule.schedule[leastBusyMachine].push_back(operations.back());
    leastBusyMachines.push({leastBusyMachineReadyTime + operations.back()->duration, leastBusyMachine});
    operations.pop_back();
  }
}
