#pragma once

#include "Schedule.hpp"
#include "Scheduler.hpp"

class SJScheduler : public Scheduler<Schedule>
{
 public:
  using Scheduler<Schedule>::Scheduler;

  void schedule(Schedule& schedule, JobSP job) override;
};
