#pragma once

#include "Scheduler.hpp"
#include "Schedule.hpp"

class SAScheduler : public Scheduler<Schedule>
{
 public:
  SAScheduler(std::shared_ptr<Input> input,
              std::shared_ptr<Machines> machines,
              std::shared_ptr<IEstimator> estimator,
              unsigned iterations);

  void schedule(Schedule & schedule, JobSP job) override;

 private:
  unsigned mIterations;
};
