#include "Schedule.hpp"

Schedule::Schedule(unsigned machinesNum)
{
  schedule.resize(machinesNum);
}

std::vector<Assignation> Schedule::dispatch(long long from, long long until)
{
  std::vector<Assignation> assignations;

  for (unsigned machine = 0; machine < schedule.size(); machine++)
  {
    long long prevFinishTime = from;
    if (ongoings.find(machine) != ongoings.end())
    {
      long long finishTime = ongoings[machine].first +
          ongoings[machine].second->duration;
      if (finishTime <= until)
      {
        prevFinishTime = finishTime;
        assignations.emplace_back(finishTime, ongoings[machine].second, machine);
        ongoings.erase(machine);
      }
      else
        continue;
    }
    for (auto it = schedule[machine].begin(); it != schedule[machine].end();)
    {
      if (prevFinishTime == until)
        break;
      long long finishTime = prevFinishTime + (*it)->duration;
      if (finishTime <= until)
      {
        prevFinishTime = finishTime;
        assignations.emplace_back(finishTime, *it, machine);
        it = schedule[machine].erase(it);
      }
      else
      {
        ongoings.emplace(machine,
                         std::pair<BeginTimestamp, OperationSP>(prevFinishTime, *it));
        schedule[machine].erase(it);
        break;
      }
    }
  }

  return assignations;
}

std::vector<Assignation> Schedule::simulateDispatch(long long from) const
{
  std::vector<Assignation> assignations;

  for (unsigned machine = 0; machine < schedule.size(); machine++)
  {
    long long prevFinishTime = from;
    if (ongoings.find(machine) != ongoings.end())
    {
      long long finishTime = ongoings.at(machine).first +
          ongoings.at(machine).second->duration;
      prevFinishTime = finishTime;
      assignations.emplace_back(finishTime, ongoings.at(machine).second, machine);
    }
    for (auto it = schedule[machine].begin(); it != schedule[machine].end(); it++)
    {
      long long finishTime = prevFinishTime + (*it)->duration;
      prevFinishTime = finishTime;
      assignations.emplace_back(finishTime, *it, machine);
    }
  }

  return assignations;
}

std::tuple<Schedule::SrcMachine,
           Schedule::DstMachine,
           Schedule::SrcMachineOffset,
           Schedule::DstMachineOffset> Schedule::random_swap()
{
  unsigned srcMachine = rand() % schedule.size();
  while (schedule[srcMachine].size() <= 0)
    srcMachine = (srcMachine + 1) % schedule.size();
  unsigned dstMachine = rand() % schedule.size();
  while (schedule[dstMachine].size() <= 0)
    dstMachine = (dstMachine + 1) % schedule.size();

  unsigned srcMachineOffset = rand() % schedule[srcMachine].size();
  unsigned dstMachineOffset = rand() % schedule[dstMachine].size();

  deterministic_swap(srcMachine, dstMachine, srcMachineOffset, dstMachineOffset);

  return std::make_tuple(srcMachine, dstMachine, srcMachineOffset, dstMachineOffset);
}

void Schedule::deterministic_swap(SrcMachine& srcMachine,
                                  DstMachine& dstMachine,
                                  SrcMachineOffset& srcMachineOffset,
                                  DstMachineOffset& dstMachineOffset)
{
  std::swap(schedule[srcMachine][srcMachineOffset],
            schedule[dstMachine][dstMachineOffset]);
}

std::tuple<Schedule::SrcMachine,
           Schedule::DstMachine,
           Schedule::SrcMachineOffset,
           Schedule::DstMachineOffset> Schedule::random_move()
{
  unsigned srcMachine = rand() % schedule.size();
  while (schedule[srcMachine].size() <= 0)
    srcMachine = (srcMachine + 1) % schedule.size();
  unsigned dstMachine = rand() % schedule.size();
  if (dstMachine == srcMachine)
    dstMachine = (dstMachine + 1) % schedule.size();

  unsigned srcMachineOffset = rand() % schedule[srcMachine].size();
  unsigned dstMachineOffset = rand() % (schedule[dstMachine].size() + 1);

  deterministic_move(srcMachine, dstMachine, srcMachineOffset, dstMachineOffset);

  return std::make_tuple(srcMachine, dstMachine, srcMachineOffset, dstMachineOffset);
}

void Schedule::deterministic_move(SrcMachine& srcMachine,
                                  DstMachine& dstMachine,
                                  SrcMachineOffset& srcMachineOffset,
                                  DstMachineOffset& dstMachineOffset)
{
  if(srcMachine == dstMachine)
    return;

  schedule[dstMachine].insert(schedule[dstMachine].begin() + dstMachineOffset,
                              schedule[srcMachine][srcMachineOffset]);
  schedule[srcMachine].erase(schedule[srcMachine].begin() + srcMachineOffset);
}
