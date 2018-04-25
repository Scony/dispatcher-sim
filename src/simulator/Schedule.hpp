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
      // TODO: handle ongoing if present
      if (schedule.ongoings.find(machine) != schedule.ongoings.end())
	{
	  long long finishTime = schedule.ongoings[machine].first + schedule.ongoings[machine].second->duration;
	  if (finishTime <= until)
	    {
	      prevFinishTime = finishTime;
	      schedule.ongoings.erase(machine);
	      // TODO: append assignation
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
	    // TODO: pop & append assignation
	  }
	else
	  {
	    // TODO: move to ongoings
	    break;
	  }
      }
    }

    return assignations;
  }
};
