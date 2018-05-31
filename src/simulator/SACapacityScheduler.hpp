#pragma once

#include "Scheduler.hpp"
#include "CapacitySchedule.hpp"

class SACapacityScheduler : public Scheduler<CapacitySchedule>
{
 public:
  SACapacityScheduler(std::shared_ptr<Input> input,
                      std::shared_ptr<Machines> machines,
                      std::shared_ptr<IEstimator> estimator,
                      unsigned iterations);

  void schedule(CapacitySchedule & schedule, JobSP job) override;

 private:
  unsigned mIterations;
};
