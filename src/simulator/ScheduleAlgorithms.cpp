#include <cassert>
#include <algorithm>
#include <queue>

#include "ScheduleAlgorithms.hpp"

void schedule::algorithm::fifo(Schedule & schedule, JobSP job)
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

void schedule::algorithm::shortestJob(Schedule & schedule, JobSP job, IEstimatorSP estimator)
{
  fifo(schedule, job);

  using JobWeight = long long;
  std::unordered_map<JobID, JobWeight> jobWeights;

  for (const auto& vec : schedule.schedule)
    for (const auto& operation : vec)
    {
      if (jobWeights.find(operation->parentId) == jobWeights.end())
        jobWeights[operation->parentId] = 0;
      jobWeights[operation->parentId] += estimator->estimate(operation);
    }

  for (auto& vec : schedule.schedule)
    std::sort(vec.begin(), vec.end(), [&](OperationSP& a, OperationSP& b) {
        return jobWeights[a->parentId] < jobWeights[b->parentId];  // ASC
      });
}

void schedule::algorithm::shortestJobLongestOperation(Schedule & schedule,
                                                      JobSP job,
                                                      IEstimatorSP estimator)
{
  const unsigned machinesNum = schedule.schedule.size();

  fifo(schedule, job);

  using JobWeight = long long;
  std::unordered_map<JobID, JobWeight> jobWeights;

  for (const auto& vec : schedule.schedule)
    for (const auto& operation : vec)
    {
      if (jobWeights.find(operation->parentId) == jobWeights.end())
        jobWeights[operation->parentId] = 0;
      jobWeights[operation->parentId] += estimator->estimate(operation);
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
        return estimator->estimate(a) < estimator->estimate(b);  // LO
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
          estimator->estimate(schedule.ongoings[machine].second);
      leastBusyMachines.push({ongoingFinish - job->arrivalTimestamp, machine});
    }
  }

  while (operations.size() > 0)
  {
    ReadyTime leastBusyMachineReadyTime = leastBusyMachines.top().first;
    MachineID leastBusyMachine = leastBusyMachines.top().second;
    leastBusyMachines.pop();
    schedule.schedule[leastBusyMachine].push_back(operations.back());
    leastBusyMachines.push({leastBusyMachineReadyTime + estimator->estimate(operations.back()),
            leastBusyMachine});
    operations.pop_back();
  }
}
