#include <algorithm>
#include <cassert>
#include <map>
#include <numeric>
#include <unordered_map>

#include <boost/icl/split_interval_map.hpp>

#include "Solution.hpp"

void Solution::handleNotification(const Assignation& notification)
{
  mSolutionVec.push_back(notification);
}

Solution::JobFlowVec Solution::calculateJobFlowVec(
    const SolutionVec& solution,
    std::vector<std::shared_ptr<Job>> jobs)
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

  std::vector<std::pair<long long, std::shared_ptr<Job>>> jobFlows;

  for (auto& job : jobs)
    jobFlows.push_back({jobEnds[job->id] - job->arrivalTimestamp, job});

  return jobFlows;
}

Solution::JobStretchVec Solution::calculateJobTStretchVec(
    const SolutionVec& solution,
    std::vector<JobSP> jobs)
{
  JobStretchVec result;

  auto flowVec = Solution::calculateJobFlowVec(solution, jobs);
  for (const auto& kv : flowVec)
  {
    const auto& flow = kv.first;
    const auto& job = kv.second;
    double jobDuration = std::accumulate(
        job->operations.begin(), job->operations.end(), 0.0d, [](double acc, OperationSP op) {
          return acc + op->duration;
        });
    result.emplace_back((double)flow / jobDuration, job);
  }

  return result;
}

Solution::JobStretchVec Solution::calculateJobMStretchVec(
    const SolutionVec& solution,
    std::vector<JobSP> jobs)
{
  JobStretchVec result;

  auto flowVec = Solution::calculateJobFlowVec(solution, jobs);
  for (const auto& pair : flowVec)
  {
    const auto& flow = pair.first;
    const auto& job = pair.second;
    double maxOperationDuration = std::accumulate(
        job->operations.begin(), job->operations.end(), 0.0d, [](double acc, OperationSP op) {
          return acc < op->duration ? op->duration : acc;
        });
    result.emplace_back((double)flow / maxOperationDuration, job);
  }

  return result;
}

Solution::JobStretchVec Solution::calculateJobWStretchVec(
    const SolutionVec& solution,
    std::vector<JobSP> jobs,
    std::shared_ptr<Machines> machines)
{
  JobStretchVec result;

  auto flowVec = Solution::calculateJobFlowVec(solution, jobs);
  for (const auto& pair : flowVec)
  {
    const auto& flow = pair.first;
    const auto& job = pair.second;
    double jobDuration = std::accumulate(
        job->operations.begin(), job->operations.end(), 0.0d, [](double acc, OperationSP op) {
          return acc + op->duration;
        });
    result.emplace_back((double)flow / (jobDuration / machines->size()), job);
  }

  return result;
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

    if (endTimestamp - operation->duration < operation->arrival)
      return false;
  }
  return true;
}

bool Solution::validateSingularOperationExecutions(
    const SolutionVec& solution,
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

bool Solution::validateMachineCapacityUsage(
    const SolutionVec& solution,
    const std::vector<MachineSP>& machines)
{
  std::unordered_map<MachineID, MachineSP> machinesMap;
  for (auto const& machine : machines)
    machinesMap.emplace(machine->id, machine);

  using Timestamp = long long;
  using Capacity = long long;
  std::unordered_map<MachineID, boost::icl::interval_map<Timestamp, Capacity>> intervalTrees;
  for (const auto& tuple : solution)
  {
    const auto& endTimestamp = std::get<0>(tuple);
    const auto& operation = std::get<1>(tuple);
    const auto& machineId = std::get<2>(tuple);

    if (machinesMap.find(machineId) == machinesMap.end())
      return false;
    if (intervalTrees.find(machineId) == intervalTrees.end())
      intervalTrees.emplace(machineId, boost::icl::interval_map<Timestamp, Capacity>{});

    auto beginTimestamp = endTimestamp - operation->duration;
    auto interval =
        boost::icl::discrete_interval<Timestamp>::right_open(beginTimestamp, endTimestamp);
    intervalTrees[machineId] += std::make_pair(interval, operation->capacityReq);
  }
  for (auto const& machine : machines)
  {
    for (auto it = intervalTrees[machine->id].begin(); it != intervalTrees[machine->id].end(); it++)
    {
      const auto& capacityUsed = it->second;
      if (capacityUsed > machine->capacity)
        return false;
    }
  }

  return true;
}
