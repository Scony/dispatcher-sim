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

  auto costFunction = [&](const CapacitySchedule& solution) {
    return Solution::evalTotalFlow(solution.simulateDispatch(job->arrivalTimestamp, mEstimator));
  };
  auto neighbouringSolution = [&](CapacitySchedule& solution) {
    if (rand() % 2 > 0)
      utility::algorithm::random_swap(solution.schedule);
    else
      utility::algorithm::random_move(solution.schedule);
  };

  auto result = algorithm::sa<CapacitySchedule, long long>(schedule,
                                                           costFunction,
                                                           neighbouringSolution,
                                                           mIterations);
  schedule = result;
}
