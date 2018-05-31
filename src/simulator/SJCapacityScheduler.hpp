#pragma once

#include "Scheduler.hpp"
#include "CapacitySchedule.hpp"

class SJCapacityScheduler : public Scheduler<CapacitySchedule>
{
 public:
  using Scheduler<CapacitySchedule>::Scheduler;

  void schedule(CapacitySchedule & schedule, JobSP job) override;
};
