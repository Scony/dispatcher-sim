#include <cassert>
#include <algorithm>

#include "SAScheduler.hpp"
#include "Algorithm.hpp"
#include "Solution.hpp"
#include "ScheduleAlgorithms.hpp"

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

  // ScheduleAlgorithms::fifo(schedule, job);
  // ScheduleAlgorithms::shortest_job(schedule, job);
  ScheduleAlgorithms::shortest_job_longest_operation(schedule, job);

  bool swap;
  std::tuple<unsigned, unsigned, unsigned, unsigned> prevMove;
  bool cacheInitialized = false;
  std::vector<Schedule::MachineCache> machineCaches(machinesNum);
  Schedule::MachineCache oldSrcMachineCache;
  Schedule::MachineCache oldDstMachineCache;
  long long prevFlow;
  long long flow;
  auto costFunction = [&](const Schedule& solution)
      {
        if (!cacheInitialized)
        {
          for (unsigned machine = 0; machine < machinesNum; machine++)
            machineCaches[machine] = solution.simulateDispatchMachine(job->arrivalTimestamp, machine);
          cacheInitialized = true;
          flow = Schedule::calculateFlowFromCache(machineCaches, mInput);
        }
        return flow;
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

        prevFlow = flow;
        flow = Schedule::calculateFlowFromCache(machineCaches, mInput);
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
        flow = prevFlow;
      };

  Algorithm::sa_inplace<Schedule, long long>(schedule,
                                             costFunction,
                                             invertSolution,
                                             revertSolution,
                                             mIterations);
}
