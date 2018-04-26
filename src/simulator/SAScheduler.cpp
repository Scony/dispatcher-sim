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

  auto costFunction = [&](const Schedule& solution)
      {
        return Solution::evalTotalFlow(solution.simulateDispatch(job->arrivalTimestamp));
      };

  bool swap;
  std::tuple<unsigned, unsigned, unsigned, unsigned> prevMove;
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
      };

  Algorithm::sa_inplace<Schedule, long long>(schedule,
                                             costFunction,
                                             invertSolution,
                                             revertSolution,
                                             mIterations);
}
