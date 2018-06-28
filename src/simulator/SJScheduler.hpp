#pragma once

#include "Scheduler.hpp"
#include "Schedule.hpp"

class SJScheduler : public Scheduler<Schedule>
{
 public:
  using Scheduler<Schedule>::Scheduler;

  void schedule(Schedule & schedule, JobSP job) override;
};
