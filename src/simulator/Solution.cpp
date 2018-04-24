#include <map>
#include <unordered_map>
#include <cassert>
#include <algorithm>

#include "Solution.hpp"

void Solution::handleNotification(const Assignation& notification)
{
  mSolutionVec.push_back(notification);
}

Solution::JobFlowVec Solution::calculateJobFlowVec(const SolutionVec& solution,
                                                   std::vector<std::shared_ptr<Job> > jobs)
{
  std::map<long long, long long> jobEnds;

  for (const auto& tuple : solution)
  {
    const auto& endTimestamp = std::get<0>(tuple);
    const auto& operation = std::get<1>(tuple);

    if (jobEnds.find(operation->parentId) == jobEnds.end())
      jobEnds[operation->parentId] = 0;

    jobEnds[operation->parentId] = std::max(jobEnds[operation->parentId], endTimestamp);
  }

  std::vector<std::pair<long long, std::shared_ptr<Job> > > jobFlows;

  for (auto& job : jobs)
    jobFlows.push_back({jobEnds[job->id] - job->arrivalTimestamp, job});

  return jobFlows;
}

long long Solution::evalTotalFlow(const SolutionVec& solution)
{
  std::unordered_map<long long, long long> jobFlows;

  for (const auto& tuple : solution)
  {
    const auto& endTimestamp = std::get<0>(tuple);
    const auto& operation = std::get<1>(tuple);

    auto iterator = jobFlows.find(operation->parentId);
    if (iterator == jobFlows.end())
    {
      auto tmp = jobFlows.emplace(operation->parentId, 0);
      iterator = tmp.first;
    }
    auto& jobFlow = iterator->second;

    jobFlow = std::max(jobFlow, endTimestamp - operation->arrival);
  }

  long long totalFlow = 0;
  for (auto const& kv : jobFlows)
    totalFlow += kv.second;

  return totalFlow;
}

bool Solution::validateOperationEnds(const SolutionVec& solution)
{
  for (const auto& tuple : solution)
  {
    const auto& endTimestamp = std::get<0>(tuple);
    const auto& operation = std::get<1>(tuple);

    if(endTimestamp - operation->duration < operation->arrival)
      return false;
  }
  return true;
}

bool Solution::validateSingularOperationExecutions(const SolutionVec& solution,
						   const std::vector<JobSP>& jobs)
{
  std::map<long long, unsigned> operationExecutions;
  for (const auto& job : jobs)
    for (const auto& operation : job->operations)
      operationExecutions[operation->id] = 0;
  for (const auto& tuple : solution)
  {
    const auto& operation = std::get<1>(tuple);
    operationExecutions[operation->id]++;
  }
  for (const auto& kv : operationExecutions)
  {
    const auto& executionsNum = kv.second;
    if (executionsNum != 1)
      return false;
  }
  return true;
}

bool Solution::validateMachineCapacityUsage(const SolutionVec& solution,
					    const std::vector<MachineSP>& machines)
{
  // FIXME: impl based on interval tree

  using MachineID = long long;
  using Timestamp = long long;
  using CapacityUsage = long long;
  std::map<MachineID, std::map<Timestamp, CapacityUsage> > usages;
  std::map<MachineID, MachineSP> machinesMap;

  for (auto const& machine : machines)
  {
    usages.emplace(machine->id, std::map<Timestamp, CapacityUsage>{});
    machinesMap.emplace(machine->id, machine);
  }

  for (const auto& tuple : solution)
  {
    const auto& endTimestamp = std::get<0>(tuple);
    const auto& operation = std::get<1>(tuple);
    const auto& machineId = std::get<2>(tuple);

    if (usages.find(machineId) == usages.end())
      return false;

    auto beginTimestamp = endTimestamp - operation->duration;
    for (auto timestamp = beginTimestamp; timestamp < endTimestamp; timestamp++)
    {
      if (usages[machineId].find(timestamp) == usages[machineId].end())
        usages[machineId][timestamp] = 0;
      if (usages[machineId][timestamp] + operation->capacityReq
          >
          machinesMap[machineId]->capacity)
        return false;
      else
        usages[machineId][timestamp] += operation->capacityReq;
    }
  }

  return true;
}
