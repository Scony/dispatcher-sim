#pragma once

#include <memory>

#include "Input.hpp"
#include "IEstimator.hpp"
#include "Machines.hpp"

template <class TSchedule>
class Scheduler
{
 public:
  Scheduler(std::shared_ptr<Input> input,
            std::shared_ptr<Machines> machines,
            std::shared_ptr<IEstimator> estimator) :
      mInput(input),
      mMachines(machines),
      mEstimator(estimator) {}
  virtual ~Scheduler() {};

  virtual void schedule(TSchedule & schedule, JobSP job) = 0;

 protected:
  std::shared_ptr<Input> mInput;
  std::shared_ptr<Machines> mMachines;
  std::shared_ptr<IEstimator> mEstimator;
};
