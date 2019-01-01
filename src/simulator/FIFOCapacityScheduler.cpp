#include "FIFOCapacityScheduler.hpp"

void FIFOCapacityScheduler::schedule(CapacitySchedule& schedule, JobSP job)
{
  auto operations = job->operations;

  unsigned machine = 0;
  while (operations.size() > 0)
  {
    for (auto it = operations.begin(); it != operations.end(); it++)
    {
      const auto& operation = *it;
      if (operation->capacityReq <= mMachines->getMachine(machine)->capacity)
      {
        schedule.schedule[machine].push_back(operation);
        operations.erase(it);
        break;
      }
    }
    machine = (machine + 1) % mMachines->size();
  }
}
