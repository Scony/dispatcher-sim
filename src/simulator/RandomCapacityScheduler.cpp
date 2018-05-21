#include <cassert>
#include <algorithm>

#include "RandomCapacityScheduler.hpp"

RandomCapacityScheduler::RandomCapacityScheduler(std::shared_ptr<Input> input,
                                                 std::shared_ptr<Machines> machines,
                                                 std::shared_ptr<IEstimator> estimator) :
    Scheduler(input, machines, estimator)
{
}

void RandomCapacityScheduler::schedule(CapacitySchedule & schedule, JobSP job)
{
  const unsigned machinesNum = schedule.schedule.size();
  assert(machinesNum > 0);

  for (auto& operation : job->operations)
  {
    int machineId = rand() % machinesNum;
    schedule.schedule[machineId].push_back(operation);
  }

  for (auto& machineVec : schedule.schedule)
    std::random_shuffle(machineVec.begin(), machineVec.end());
}
