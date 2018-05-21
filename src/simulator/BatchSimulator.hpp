#pragma once

#include <deque>
#include <algorithm>
#include <climits>

#include "Input.hpp"
#include "Scheduler.hpp"
#include "Types.hpp"
#include "ExecutionsSubject.hpp"
#include "Machines.hpp"

template <class TSchedule>
class BatchSimulator : public ExecutionsSubject
{
 public:
  BatchSimulator(std::shared_ptr<Input> input,
                 std::shared_ptr<Machines> machines,
                 std::shared_ptr<Scheduler<TSchedule> > scheduler) :
      mMachines(machines),
      mScheduler(scheduler),
      mSchedule(mMachines)
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

  void run()
  {
    long long previousTimestamp = 0;

    while (mQueue.size() > 0)
    {
      auto newestJob = mQueue.back();
      auto partialSolution = mSchedule.dispatch(previousTimestamp, newestJob->arrivalTimestamp);
      for (const auto& assignation : partialSolution)
        notify(assignation);
      previousTimestamp = newestJob->arrivalTimestamp;
      mScheduler->schedule(mSchedule, newestJob);
      mQueue.pop_back();
    }

    auto partialSolution = mSchedule.dispatch(previousTimestamp, LLONG_MAX);
    for (const auto& assignation : partialSolution)
      notify(assignation);
  }

 private:
  std::shared_ptr<Machines> mMachines;

  std::shared_ptr<Scheduler<TSchedule> > mScheduler;
  std::deque<std::shared_ptr<Job> > mQueue;
  TSchedule mSchedule;
};
