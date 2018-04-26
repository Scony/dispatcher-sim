#pragma once

#include <vector>
#include <climits>
#include <unordered_map>

#include "Operation.hpp"
#include "Types.hpp"
#include "Input.hpp"

struct Schedule
{
  Schedule(unsigned machinesNum);

  std::vector<Assignation> dispatch(long long from, long long until);
  std::vector<Assignation> simulateDispatch(long long from) const;

  using JobFinish = long long;
  using MachineCache = std::unordered_map<JobID, JobFinish>;
  MachineCache simulateDispatchMachine(long long from, MachineID machine) const;
  using Cache = std::unordered_map<MachineID, Schedule::MachineCache>;
  static long long calculateFlowFromCache(const Cache& machineCaches, std::shared_ptr<Input> input);

  using SrcMachine = unsigned;
  using DstMachine = unsigned;
  using SrcMachineOffset = unsigned;
  using DstMachineOffset = unsigned;
  std::tuple<SrcMachine, DstMachine, SrcMachineOffset, DstMachineOffset> random_swap();
  void deterministic_swap(SrcMachine& srcMachine,
                          DstMachine& dstMachine,
                          SrcMachineOffset& srcMachineOffset,
                          DstMachineOffset& dstMachineOffset);
  std::tuple<SrcMachine, DstMachine, SrcMachineOffset, DstMachineOffset> random_move();
  void deterministic_move(SrcMachine& srcMachine,
                          DstMachine& dstMachine,
                          SrcMachineOffset& srcMachineOffset,
                          DstMachineOffset& dstMachineOffset);

  using BeginTimestamp = long long;
  std::unordered_map<MachineID, std::pair<BeginTimestamp, OperationSP> > ongoings;
  std::vector<std::vector<OperationSP> > schedule;
};