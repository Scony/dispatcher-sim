#include <boost/icl/split_interval_map.hpp>

#include "CapacitySchedule.hpp"

CapacitySchedule::CapacitySchedule(std::shared_ptr<Machines> machines)
    : ongoings{machines->size()}
    , schedule{machines->size()}
    , machines{machines}
{
}

std::vector<Assignation> CapacitySchedule::dispatch(long long from, long long until)
{
  std::vector<Assignation> assignations;

  for (unsigned machineId = 0; machineId < machines->size(); machineId++)
  {
    const auto machineCapacity = machines->getMachine(machineId)->capacity;
    using Timestamp = long long;
    using Capacity = long long;
    boost::icl::interval_map<Timestamp, Capacity> intervalCapacityUsages;

    for (auto it = ongoings[machineId].begin(); it != ongoings[machineId].end();)
    {
      auto& beginTimestamp = it->first;
      auto& operation = it->second;
      auto endTimestamp = beginTimestamp + operation->duration;

      auto interval = boost::icl::discrete_interval<Timestamp>::right_open(from, endTimestamp);
      intervalCapacityUsages += std::make_pair(interval, operation->capacityReq);

      if (endTimestamp <= until)
      {
        assignations.emplace_back(endTimestamp, operation, machineId);
        it = ongoings[machineId].erase(it);
      }
      else
        it++;
    }

    for (auto it = schedule[machineId].begin(); it != schedule[machineId].end();)
    {
      auto& operation = *it;
      auto beginTimestamp = from;

      for (auto it = intervalCapacityUsages.begin(); it != intervalCapacityUsages.end(); it++)
      {
        const auto& interval = it->first;
        const auto& capacityUsed = it->second;
        if (capacityUsed + operation->capacityReq <= machineCapacity)
        {
          if (beginTimestamp + operation->duration <= interval.upper())
            break;
          else
            continue;
        }
        else
          beginTimestamp = interval.upper();
      }

      auto endTimestamp = beginTimestamp + operation->duration;
      auto interval = boost::icl::discrete_interval<Timestamp>::right_open(beginTimestamp,
                                                                           endTimestamp);
      intervalCapacityUsages += std::make_pair(interval, operation->capacityReq);

      if (beginTimestamp < until)
      {
        if (endTimestamp <= until)
          assignations.emplace_back(endTimestamp, operation, machineId);
        else
          ongoings[machineId].emplace_back(beginTimestamp, operation);
        it = schedule[machineId].erase(it);
      }
      else
        it++;
    }
  }

  return assignations;
}

std::vector<Assignation> CapacitySchedule::simulateDispatch(long long from,
                                                            IEstimatorSP estimator) const
{
  std::vector<Assignation> assignations;

  for (unsigned machineId = 0; machineId < machines->size(); machineId++)
  {
    const auto machineCapacity = machines->getMachine(machineId)->capacity;
    using Timestamp = long long;
    using Capacity = long long;
    boost::icl::interval_map<Timestamp, Capacity> intervalCapacityUsages;

    for (auto it = ongoings[machineId].begin(); it != ongoings[machineId].end(); it++)
    {
      auto& beginTimestamp = it->first;
      auto& operation = it->second;
      auto endTimestamp = std::max(from, beginTimestamp + estimator->estimate(operation));

      auto interval = boost::icl::discrete_interval<Timestamp>::right_open(from, endTimestamp);
      intervalCapacityUsages += std::make_pair(interval, operation->capacityReq);
    }

    for (auto it = schedule[machineId].begin(); it != schedule[machineId].end(); it++)
    {
      auto& operation = *it;
      auto beginTimestamp = from;

      for (auto it = intervalCapacityUsages.begin(); it != intervalCapacityUsages.end(); it++)
      {
        const auto& interval = it->first;
        const auto& capacityUsed = it->second;
        if (capacityUsed + operation->capacityReq <= machineCapacity)
        {
          if (beginTimestamp + estimator->estimate(operation) <= interval.upper())
            break;
          else
            continue;
        }
        else
          beginTimestamp = interval.upper();
      }

      auto endTimestamp = beginTimestamp + estimator->estimate(operation);
      auto interval = boost::icl::discrete_interval<Timestamp>::right_open(beginTimestamp,
                                                                           endTimestamp);
      intervalCapacityUsages += std::make_pair(interval, operation->capacityReq);
      assignations.emplace_back(endTimestamp, operation, machineId);
    }
  }

  return assignations;
}

CapacitySchedule::MachineCache CapacitySchedule::simulateDispatchMachine(long long from,
                                                                         MachineID machineId,
                                                                         IEstimatorSP estimator) const
{
  const auto machineCapacity = machines->getMachine(machineId)->capacity;
  MachineCache cache;

  using Timestamp = long long;
  using Capacity = long long;
  boost::icl::interval_map<Timestamp, Capacity> intervalCapacityUsages;

  for (auto it = ongoings[machineId].begin(); it != ongoings[machineId].end(); it++)
  {
    auto& beginTimestamp = it->first;
    auto& operation = it->second;
    auto endTimestamp = std::max(from, beginTimestamp + estimator->estimate(operation));

    auto interval = boost::icl::discrete_interval<Timestamp>::right_open(from, endTimestamp);
    intervalCapacityUsages += std::make_pair(interval, operation->capacityReq);
  }

  for (auto it = schedule[machineId].begin(); it != schedule[machineId].end(); it++)
  {
    auto& operation = *it;
    auto beginTimestamp = from;

    for (auto it = intervalCapacityUsages.begin(); it != intervalCapacityUsages.end(); it++)
    {
      const auto& interval = it->first;
      const auto& capacityUsed = it->second;
      if (capacityUsed + operation->capacityReq <= machineCapacity)
      {
        if (beginTimestamp + estimator->estimate(operation) <= interval.upper())
          break;
        else
          continue;
      }
      else
        beginTimestamp = interval.upper();
    }

    auto endTimestamp = beginTimestamp + estimator->estimate(operation);
    auto interval = boost::icl::discrete_interval<Timestamp>::right_open(beginTimestamp,
                                                                         endTimestamp);
    intervalCapacityUsages += std::make_pair(interval, operation->capacityReq);
    auto it2 = cache.find(operation->parentId);
    if (it2 == cache.end())
      cache.emplace(operation->parentId, endTimestamp);
    else
      it2->second = std::max(it2->second, endTimestamp);
  }

  return cache;
}

long long CapacitySchedule::calculateFlowFromCache(const Cache& machineCaches,
                                                   std::shared_ptr<Input> input)
{
  std::unordered_map<JobID, JobFinish> finalJobFinishes;

  for (const auto& machineCache : machineCaches)
    for (const auto& kv2 : machineCache)
    {
      const auto& jobId = kv2.first;
      const auto& jobFinish = kv2.second;
      auto it = finalJobFinishes.find(jobId);
      if (it == finalJobFinishes.end())
        finalJobFinishes.emplace(jobId, jobFinish);
      else
        it->second = std::max(it->second, jobFinish);
    }

  long long totalFlow = 0;
  for (const auto& kv : finalJobFinishes)
  {
    const auto& jobId = kv.first;
    const auto& jobFinish = kv.second;
    long long jobFlow = jobFinish - input->getJob(jobId)->arrivalTimestamp;
    totalFlow += jobFlow;
  }

  return totalFlow;
}
