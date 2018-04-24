#pragma once

#include <deque>

#include "Input.hpp"
#include "Scheduler.hpp"
#include "Types.hpp"

template <class TSchedule>
class BatchSimulator
{
 public:
  BatchSimulator(std::shared_ptr<Input> input,
                 const std::vector<MachineSP>& machines,
                 std::shared_ptr<Scheduler<TSchedule> > scheduler) :
      mMachines(machines),
      mScheduler(scheduler)
  {
    // copy input to queue
    auto jobs = input->getJobs();
    for (auto& job : jobs)
      mQueue.push_back(job);

    // sort queue by arrival time DESC
    std::sort(mQueue.begin(), mQueue.end(), [](std::shared_ptr<Job> a, std::shared_ptr<Job> b) {
        return a->arrivalTimestamp > b->arrivalTimestamp;
      });
  }

  std::vector<Assignation> run()
  {
    std::vector<Assignation> solution;
    TSchedule::init(mSchedule, mMachines.size());
    long long previousTimestamp = 0;

    while (mQueue.size() > 0)
    {
      auto newestJob = mQueue.back();
      auto partialSolution = TSchedule::dispatch(mSchedule,
                                                 previousTimestamp,
                                                 newestJob->arrivalTimestamp);
      previousTimestamp = newestJob->arrivalTimestamp;
      mScheduler->schedule(mSchedule, newestJob);
      mQueue.pop_back();
    }

    auto partialSolution = TSchedule::dispatch(mSchedule, previousTimestamp, LLONG_MAX);

    return solution;
  }

 private:
  const std::vector<MachineSP>& mMachines;

  std::shared_ptr<Scheduler<TSchedule> > mScheduler;
  std::deque<std::shared_ptr<Job> > mQueue;
  TSchedule mSchedule;
};
