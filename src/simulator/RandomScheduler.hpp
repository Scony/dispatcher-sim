#pragma once

#include "Schedule.hpp"
#include "Scheduler.hpp"

class RandomScheduler : public Scheduler<Schedule>
{
 public:
  RandomScheduler(
      std::shared_ptr<Input> input,
      std::shared_ptr<Machines> machines,
      std::shared_ptr<IEstimator> estimator);

  void schedule(Schedule& schedule, JobSP job) override;
};
