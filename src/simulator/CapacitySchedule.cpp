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
  using OperationID = long long;

  for (unsigned machine = 0; machine < machines->size(); machine++)
  {
    using Timestamp = long long;
    using Capacity = long long;
    boost::icl::interval_map<Timestamp, Capacity> intervalCapacityUsages;

    for (auto it = ongoings[machine].begin(); it != ongoings[machine].end();)
    {
      auto& beginTimestamp = it->first;
      auto& operation = it->second;
      auto endTimestamp = beginTimestamp + operation->duration;

      auto interval = boost::icl::discrete_interval<Timestamp>::right_open(from, endTimestamp);
      intervalCapacityUsages += std::make_pair(interval, operation->capacityReq);

      if (endTimestamp <= until)
      {
        assignations.emplace_back(endTimestamp, operation, machine);
        it = ongoings[machine].erase(it);
      }
      else
        it++;
    }

    for (auto it = schedule[machine].begin(); it != schedule[machine].end();)
    {
      auto& operation = *it;
      auto beginTimestamp = from;

      for (auto it = intervalCapacityUsages.begin(); it != intervalCapacityUsages.end(); it++)
      {
        const auto& interval = it->first;
        const auto& capacityUsed = it->second;
        if (capacityUsed + operation->capacityReq <= machines->getMachine(machine)->capacity)
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
          assignations.emplace_back(endTimestamp, operation, machine);
        else
          ongoings[machine].emplace_back(beginTimestamp, operation);
        it = schedule[machine].erase(it);
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
  assert(false);
}
