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
        return Solution::evalTotalFlow(Schedule::simulateDispatch(solution, job->arrivalTimestamp));
      };
  auto neighbouringSolution = [](Schedule& solution)
      {
        if (rand() % 2 > 0)  // swap
        {
          unsigned srcMachine = rand() % solution.schedule.size();
          while (solution.schedule[srcMachine].size() <= 0)
            srcMachine = (srcMachine + 1) % solution.schedule.size();
          unsigned dstMachine = rand() % solution.schedule.size();
          while (solution.schedule[dstMachine].size() <= 0)
            dstMachine = (dstMachine + 1) % solution.schedule.size();

          unsigned srcMachineOffset = rand() % solution.schedule[srcMachine].size();
          unsigned dstMachineOffset = rand() % solution.schedule[dstMachine].size();

          std::swap(solution.schedule[srcMachine][srcMachineOffset],
                    solution.schedule[dstMachine][dstMachineOffset]);
        }
        else  // move
        {
          unsigned srcMachine = rand() % solution.schedule.size();
          while (solution.schedule[srcMachine].size() <= 0)
            srcMachine = (srcMachine + 1) % solution.schedule.size();
          unsigned dstMachine = rand() % solution.schedule.size();
          if (dstMachine == srcMachine)
            dstMachine = (dstMachine + 1) % solution.schedule.size();

          unsigned srcMachineOffset = rand() % solution.schedule[srcMachine].size();
          unsigned dstMachineOffset = rand() % (solution.schedule[dstMachine].size() + 1);

          solution.schedule[dstMachine].insert(
              solution.schedule[dstMachine].begin() + dstMachineOffset,
              solution.schedule[srcMachine][srcMachineOffset]
                                               );
          solution.schedule[srcMachine].erase(solution.schedule[srcMachine].begin() +
                                              srcMachineOffset);
        }
      };

  Schedule result = Algorithm::sa<Schedule, long long>(schedule,
                                                       costFunction,
                                                       neighbouringSolution,
                                                       mIterations);
  schedule = result;
}
