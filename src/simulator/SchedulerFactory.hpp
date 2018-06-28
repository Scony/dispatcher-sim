#pragma once

#include <memory>

#include "Input.hpp"
#include "Machines.hpp"
#include "IEstimator.hpp"
#include "Arguments.hpp"
#include "Scheduler.hpp"

#include "Schedule.hpp"
#include "RandomScheduler.hpp"
#include "SAScheduler.hpp"
#include "SJScheduler.hpp"
#include "SJLOScheduler.hpp"

#include "CapacitySchedule.hpp"
#include "RandomCapacityScheduler.hpp"
#include "FIFOCapacityScheduler.hpp"
#include "SJCapacityScheduler.hpp"
#include "SJLOCapacityScheduler.hpp"
#include "SACapacityScheduler.hpp"

template <class TSchedule>
class SchedulerFactory
{
 public:
  SchedulerFactory(std::shared_ptr<Input> input,
                   std::shared_ptr<Machines> machines,
                   std::shared_ptr<IEstimator> estimator,
                   const Arguments& arguments)
      : mInput(input)
      , mMachines(machines)
      , mEstimator(estimator)
      , mArguments(arguments)
  {}

  std::shared_ptr<Scheduler<TSchedule> > create();

 private:
  std::shared_ptr<Input> mInput;
  std::shared_ptr<Machines> mMachines;
  std::shared_ptr<IEstimator> mEstimator;
  const Arguments& mArguments;
};

template <>
std::shared_ptr<Scheduler<Schedule> > SchedulerFactory<Schedule>::create()
{
  std::shared_ptr<Scheduler<Schedule> > scheduler;
  if (mArguments.primaryAlgorithm == "random")
    scheduler = std::make_shared<RandomScheduler>(mInput,
                                                  mMachines,
                                                  mEstimator);
  if (mArguments.primaryAlgorithm == "sa")
    scheduler = std::make_shared<SAScheduler>(mInput,
                                              mMachines,
                                              mEstimator,
                                              mArguments.saIterations);
  if (mArguments.primaryAlgorithm == "sj")
    scheduler = std::make_shared<SJScheduler>(mInput,
                                              mMachines,
                                              mEstimator);
  if (mArguments.primaryAlgorithm == "sjlo")
    scheduler = std::make_shared<SJLOScheduler>(mInput,
                                                mMachines,
                                                mEstimator);
  assert(scheduler != nullptr && "Cannot create such algorithm for this representation");
  return scheduler;
}

template <>
std::shared_ptr<Scheduler<CapacitySchedule> > SchedulerFactory<CapacitySchedule>::create()
{
  std::shared_ptr<Scheduler<CapacitySchedule> > scheduler;
  if (mArguments.primaryAlgorithm == "random")
    scheduler = std::make_shared<RandomCapacityScheduler>(mInput,
                                                          mMachines,
                                                          mEstimator);
  if (mArguments.primaryAlgorithm == "fifo")
    scheduler = std::make_shared<FIFOCapacityScheduler>(mInput,
                                                        mMachines,
                                                        mEstimator);
  if (mArguments.primaryAlgorithm == "sj")
    scheduler = std::make_shared<SJCapacityScheduler>(mInput,
                                                      mMachines,
                                                      mEstimator);
  if (mArguments.primaryAlgorithm == "sjlo")
    scheduler = std::make_shared<SJLOCapacityScheduler>(mInput,
                                                        mMachines,
                                                        mEstimator);
  if (mArguments.primaryAlgorithm == "sa")
    scheduler = std::make_shared<SACapacityScheduler>(mInput,
                                                      mMachines,
                                                      mEstimator,
                                                      mArguments.saIterations);
  assert(scheduler != nullptr && "Cannot create such algorithm for this representation");
  return scheduler;
}
