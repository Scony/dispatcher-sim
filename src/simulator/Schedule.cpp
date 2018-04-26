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

Schedule::MachineCache Schedule::simulateDispatchMachine(long long from, MachineID machine) const
{
  MachineCache cache;

  long long prevFinishTime = from;
  if (ongoings.find(machine) != ongoings.end())
  {
    long long finishTime = ongoings.at(machine).first +
        ongoings.at(machine).second->duration;
    prevFinishTime = finishTime;
    auto& operation = ongoings.at(machine).second;
    auto it = cache.find(operation->parentId);
    if (it == cache.end())
      cache.emplace(operation->parentId, finishTime);
    else
      it->second = std::max(it->second, finishTime);
  }
  for (auto it = schedule[machine].begin(); it != schedule[machine].end(); it++)
  {
    auto& operation = *it;
    long long finishTime = prevFinishTime + operation->duration;
    prevFinishTime = finishTime;
    auto it2 = cache.find(operation->parentId);
    if (it2 == cache.end())
      cache.emplace(operation->parentId, finishTime);
    else
      it2->second = std::max(it2->second, finishTime);
  }

  return cache;
}

long long Schedule::calculateFlowFromCache(const Cache& machineCaches, std::shared_ptr<Input> input)
{
  std::unordered_map<JobID, JobFinish> finalJobFinishes;

  for (const auto& kv : machineCaches)
  {
    const auto& machineCache = kv.second;
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
