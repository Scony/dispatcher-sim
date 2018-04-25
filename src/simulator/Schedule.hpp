#pragma once

#include <vector>
#include <climits>
#include <unordered_map>

#include "Operation.hpp"

struct Schedule
{
  using BeginTimestamp = long long;
  std::unordered_map<MachineID, std::pair<BeginTimestamp, OperationSP> > ongoings;
  std::vector<std::vector<OperationSP> > schedule;

  static void init(Schedule& schedule, unsigned machinesNum)
  {
    schedule.schedule.resize(machinesNum);
  }

  static std::vector<Assignation> dispatch(Schedule& schedule, long long from, long long until)
  {
    std::vector<Assignation> assignations;

    for (unsigned machine = 0; machine < schedule.schedule.size(); machine++)
    {
      long long prevFinishTime = from;
      if (schedule.ongoings.find(machine) != schedule.ongoings.end())
      {
        long long finishTime = schedule.ongoings[machine].first +
            schedule.ongoings[machine].second->duration;
        if (finishTime <= until)
        {
          prevFinishTime = finishTime;
          assignations.emplace_back(finishTime, schedule.ongoings[machine].second, machine);
          schedule.ongoings.erase(machine);
        }
        else
          continue;
      }
      for (auto it = schedule.schedule[machine].begin(); it != schedule.schedule[machine].end();)
      {
	if (prevFinishTime == until)
	  break;
	long long finishTime = prevFinishTime + (*it)->duration;
	if (finishTime <= until)
        {
          prevFinishTime = finishTime;
          assignations.emplace_back(finishTime, *it, machine);
          it = schedule.schedule[machine].erase(it);
        }
	else
        {
          schedule.ongoings.emplace(machine,
                                    std::pair<BeginTimestamp, OperationSP>(prevFinishTime, *it));
          schedule.schedule[machine].erase(it);
          break;
        }
      }
    }

    return assignations;
  }

  static std::vector<Assignation> simulateDispatch(const Schedule& schedule, long long from)
  {
    std::vector<Assignation> assignations;

    for (unsigned machine = 0; machine < schedule.schedule.size(); machine++)
    {
      long long prevFinishTime = from;
      if (schedule.ongoings.find(machine) != schedule.ongoings.end())
      {
        long long finishTime = schedule.ongoings.at(machine).first +
            schedule.ongoings.at(machine).second->duration;
        prevFinishTime = finishTime;
        assignations.emplace_back(finishTime, schedule.ongoings.at(machine).second, machine);
      }
      for (auto it = schedule.schedule[machine].begin(); it != schedule.schedule[machine].end(); it++)
      {
	long long finishTime = prevFinishTime + (*it)->duration;
        prevFinishTime = finishTime;
        assignations.emplace_back(finishTime, *it, machine);
      }
    }

    return assignations;
  }
};
