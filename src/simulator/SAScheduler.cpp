#include <cassert>
#include <algorithm>

#include "SAScheduler.hpp"
#include "Algorithm.hpp"
#include "Solution.hpp"

SAScheduler::SAScheduler(std::shared_ptr<Input> input,
                         std::shared_ptr<Machines> machines,
                         std::shared_ptr<IEstimator> estimator,
                         unsigned iterations) :
    Scheduler(input, machines, estimator),
    mIterations(iterations)
{
}

void SAScheduler::schedule(Schedule & schedule, JobSP job)
{
  const unsigned machinesNum = schedule.schedule.size();
  assert(machinesNum > 0);

  unsigned machineId = 0;
  for (auto& operation : job->operations)
  {
    schedule.schedule[machineId].push_back(operation);
    machineId = (machineId + 1) % machinesNum;
  }

  bool swap;
  std::tuple<unsigned, unsigned, unsigned, unsigned> prevMove;
  bool cacheInitialized = false;
  std::unordered_map<MachineID, Schedule::MachineCache> machineCaches; // TODO: vector
  Schedule::MachineCache oldSrcMachineCache;
  Schedule::MachineCache oldDstMachineCache;
  auto costFunction = [&](const Schedule& solution)
      {
        if (!cacheInitialized)
        {
          for (unsigned machine = 0; machine < machinesNum; machine++)
            machineCaches[machine] = solution.simulateDispatchMachine(job->arrivalTimestamp, machine);
          cacheInitialized = true;
        }
        return Schedule::calculateFlowFromCache(machineCaches, mInput); // TODO: recalculate only if new
      };
  auto invertSolution = [&](Schedule& solution)
      {
        if (rand() % 2 > 0)
        {
          swap = true;
          prevMove = solution.random_swap();
        }
        else
        {
          swap = false;
          prevMove = solution.random_move();
        }

        unsigned& srcMachine = std::get<0>(prevMove);
        unsigned& dstMachine = std::get<1>(prevMove);
        if (srcMachine != dstMachine)
        {
          oldSrcMachineCache = machineCaches[srcMachine];
          oldDstMachineCache = machineCaches[dstMachine];
          machineCaches[srcMachine] = solution.simulateDispatchMachine(job->arrivalTimestamp,
                                                                       srcMachine);
          machineCaches[dstMachine] = solution.simulateDispatchMachine(job->arrivalTimestamp,
                                                                       dstMachine);
        }
        else
        {
          oldSrcMachineCache = machineCaches[srcMachine];
          machineCaches[srcMachine] = solution.simulateDispatchMachine(job->arrivalTimestamp,
                                                                       srcMachine);
        }
      };
  auto revertSolution = [&](Schedule& solution)
      {
        unsigned& srcMachine = std::get<1>(prevMove);
        unsigned& dstMachine = std::get<0>(prevMove);
        unsigned& srcMachineOffset = std::get<3>(prevMove);
        unsigned& dstMachineOffset = std::get<2>(prevMove);
        if (swap)
          solution.deterministic_swap(srcMachine, dstMachine, srcMachineOffset, dstMachineOffset);
        else
          solution.deterministic_move(srcMachine, dstMachine, srcMachineOffset, dstMachineOffset);

        unsigned& oldSrcMachine = std::get<0>(prevMove);
        unsigned& oldDstMachine = std::get<1>(prevMove);
        if (srcMachine != dstMachine)
        {
          machineCaches[oldSrcMachine] = oldSrcMachineCache;
          machineCaches[oldDstMachine] = oldDstMachineCache;
        }
        else
        {
          machineCaches[oldSrcMachine] = oldSrcMachineCache;
        }
      };

  Algorithm::sa_inplace<Schedule, long long>(schedule,
                                             costFunction,
                                             invertSolution,
                                             revertSolution,
                                             mIterations);
}
