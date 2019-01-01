#pragma once

#include "CapacitySchedule.hpp"
#include "Scheduler.hpp"

class SJLOCapacityScheduler : public Scheduler<CapacitySchedule>
{
 public:
  using Scheduler<CapacitySchedule>::Scheduler;

  void schedule(CapacitySchedule& schedule, JobSP job) override;
};
