#include "SACapacityScheduler.hpp"
#include "SJCapacityScheduler.hpp"
#include "Solution.hpp"
#include "Utility.hpp"
#include "Algorithm.hpp"

SACapacityScheduler::SACapacityScheduler(std::shared_ptr<Input> input,
                                         std::shared_ptr<Machines> machines,
                                         std::shared_ptr<IEstimator> estimator,
                                         unsigned iterations)
    : Scheduler(input, machines, estimator)
    , mIterations(iterations)
{
}

void SACapacityScheduler::schedule(CapacitySchedule & schedule, JobSP job)
{
  SJCapacityScheduler sj(mInput, mMachines, mEstimator);
  sj.schedule(schedule, job);

  bool swap;
  std::tuple<unsigned, unsigned, unsigned, unsigned> prevMove;
  bool initialized = false;
  std::vector<CapacitySchedule::MachineCache> machineCaches(mMachines->size());
  CapacitySchedule::MachineCache oldSrcMachineCache;
  CapacitySchedule::MachineCache oldDstMachineCache;
  long long prevFlow;
  long long flow;

  auto costFunction = [&](const CapacitySchedule& solution) {
    if (!initialized)
    {
      for (unsigned machine = 0; machine < mMachines->size(); machine++)
        machineCaches[machine] = solution.simulateDispatchMachine(job->arrivalTimestamp,
                                                                  machine,
                                                                  mEstimator);
      initialized = true;
      flow = CapacitySchedule::calculateFlowFromCache(machineCaches, mInput);
    }
    return flow;
  };
  auto changeIsValid = [&](CapacitySchedule& solution) {
    unsigned& srcMachine = std::get<0>(prevMove);
    unsigned& dstMachine = std::get<1>(prevMove);
    unsigned& srcMachineOffset = std::get<2>(prevMove);
    unsigned& dstMachineOffset = std::get<3>(prevMove);
    if (swap)
    {
      if (mMachines->getMachine(srcMachine)->capacity <
          solution.schedule[srcMachine][srcMachineOffset]->capacityReq)
        return false;
      if (mMachines->getMachine(dstMachine)->capacity <
          solution.schedule[dstMachine][dstMachineOffset]->capacityReq)
        return false;
    }
    else
    {
      if (mMachines->getMachine(dstMachine)->capacity <
          solution.schedule[dstMachine][dstMachineOffset]->capacityReq)
        return false;
    }
    return true;
  };
  auto revertMove = [&](CapacitySchedule& solution) {
    unsigned& srcMachine = std::get<1>(prevMove);
    unsigned& dstMachine = std::get<0>(prevMove);
    unsigned& srcMachineOffset = std::get<3>(prevMove);
    unsigned& dstMachineOffset = std::get<2>(prevMove);
    if (swap)
      utility::algorithm::deterministic_swap(solution.schedule,
                                             srcMachine,
                                             dstMachine,
                                             srcMachineOffset,
                                             dstMachineOffset);
    else
      utility::algorithm::deterministic_move(solution.schedule,
                                             srcMachine,
                                             dstMachine,
                                             srcMachineOffset,
                                             dstMachineOffset);
  };
  auto invertSolution = [&](CapacitySchedule& solution) {
    if (rand() % 2 > 0)
    {
      swap = true;
      prevMove = utility::algorithm::random_swap(solution.schedule);
      while (!changeIsValid(solution))
      {
        revertMove(solution);
        prevMove = utility::algorithm::random_swap(solution.schedule);
      }
    }
    else
    {
      swap = false;
      prevMove = utility::algorithm::random_move(solution.schedule);
      while (!changeIsValid(solution))
      {
        revertMove(solution);
        prevMove = utility::algorithm::random_move(solution.schedule);
      }
    }

    unsigned& srcMachine = std::get<0>(prevMove);
    unsigned& dstMachine = std::get<1>(prevMove);
    if (srcMachine != dstMachine)
    {
      oldSrcMachineCache = machineCaches[srcMachine];
      oldDstMachineCache = machineCaches[dstMachine];
      machineCaches[srcMachine] = solution.simulateDispatchMachine(job->arrivalTimestamp,
                                                                   srcMachine,
                                                                   mEstimator);
      machineCaches[dstMachine] = solution.simulateDispatchMachine(job->arrivalTimestamp,
                                                                   dstMachine,
                                                                   mEstimator);
    }
    else
    {
      oldSrcMachineCache = machineCaches[srcMachine];
      machineCaches[srcMachine] = solution.simulateDispatchMachine(job->arrivalTimestamp,
                                                                   srcMachine,
                                                                   mEstimator);
    }

    prevFlow = flow;
    flow = CapacitySchedule::calculateFlowFromCache(machineCaches, mInput);
  };
  auto revertSolution = [&](CapacitySchedule& solution) {
    unsigned& srcMachine = std::get<1>(prevMove);
    unsigned& dstMachine = std::get<0>(prevMove);
    revertMove(solution);
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

  algorithm::sa_inplace<CapacitySchedule, long long>(schedule,
                                                     costFunction,
                                                     invertSolution,
                                                     revertSolution,
                                                     mIterations);
}
