#include <cassert>
#include <algorithm>

#include "RandomCapacityScheduler.hpp"

void RandomCapacityScheduler::schedule(CapacitySchedule & schedule, JobSP job)
{
  const unsigned machinesNum = schedule.schedule.size();
  assert(machinesNum > 0);

  for (auto& operation : job->operations)
  {
    int machineId = rand() % machinesNum;
    while (operation->capacityReq > mMachines->getMachine(machineId)->capacity)
      machineId = rand() % machinesNum;
    schedule.schedule[machineId].push_back(operation);
  }

  for (auto& machineVec : schedule.schedule)
    std::random_shuffle(machineVec.begin(), machineVec.end());
}
