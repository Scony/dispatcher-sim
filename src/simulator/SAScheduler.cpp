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
          // TODO: progress in circle till valid machines found
          unsigned sourceMachine = rand() % solution.schedule.size();
          unsigned destinationMachine = rand() % solution.schedule.size();
          if (solution.schedule[sourceMachine].size() > 0 and
              solution.schedule[destinationMachine].size() > 0)
          {
            unsigned sourceMachineOffset = rand() % solution.schedule[sourceMachine].size();
            unsigned destinationMachineOffset = rand() % solution.schedule[destinationMachine].size();
            std::swap(solution.schedule[sourceMachine][sourceMachineOffset],
                      solution.schedule[destinationMachine][destinationMachineOffset]);
          }
        }
        else  // move
        {
          // TODO: progress in circle till valid machines found
          unsigned sourceMachine = rand() % solution.schedule.size();
          if (solution.schedule[sourceMachine].size() > 0)
          {
            unsigned destinationMachine = rand() % solution.schedule.size();
            if (destinationMachine == sourceMachine)  // TODO: force
              return;
            unsigned sourceMachineOffset = rand() % solution.schedule[sourceMachine].size();
            unsigned destinationMachineOffset;
            if (solution.schedule[destinationMachine].size() == 0)
              destinationMachineOffset = 0;
            else
              destinationMachineOffset = rand() % (solution.schedule[destinationMachine].size() + 1);
            solution.schedule[destinationMachine].insert(
                solution.schedule[destinationMachine].begin() + destinationMachineOffset,
                solution.schedule[sourceMachine][sourceMachineOffset]
                                                         );
            solution.schedule[sourceMachine].erase(solution.schedule[sourceMachine].begin() +
                                                   sourceMachineOffset);
          }
        }
      };

  Schedule result = Algorithm::sa<Schedule, long long>(schedule,
                                                       costFunction,
                                                       neighbouringSolution,
                                                       mIterations);
  schedule = result;
}
