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
  long long prevFlow;
  long long flow;

  auto costFunction = [&](const CapacitySchedule& solution) {
    if (!initialized)
    {
      flow = Solution::evalTotalFlow(solution.simulateDispatch(job->arrivalTimestamp, mEstimator));
      initialized = true;
    }
    return flow;
  };
  auto invertSolution = [&](CapacitySchedule& solution) {
    if (rand() % 2 > 0)
    {
      swap = true;
      prevMove = utility::algorithm::random_swap(solution.schedule);
    }
    else
    {
      swap = false;
      prevMove = utility::algorithm::random_move(solution.schedule);
    }
    prevFlow = flow;
    flow = Solution::evalTotalFlow(solution.simulateDispatch(job->arrivalTimestamp, mEstimator));
  };
  auto revertSolution = [&](CapacitySchedule& solution) {
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
    flow = prevFlow;
  };

  algorithm::sa_inplace<CapacitySchedule, long long>(schedule,
                                                     costFunction,
                                                     invertSolution,
                                                     revertSolution,
                                                     mIterations);
}
