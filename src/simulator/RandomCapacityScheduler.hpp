#pragma once

#include "Scheduler.hpp"
#include "CapacitySchedule.hpp"

class RandomCapacityScheduler : public Scheduler<CapacitySchedule>
{
 public:
  RandomCapacityScheduler(std::shared_ptr<Input> input,
                          std::shared_ptr<Machines> machines,
                          std::shared_ptr<IEstimator> estimator);

  void schedule(CapacitySchedule & schedule, JobSP job) override;
};
