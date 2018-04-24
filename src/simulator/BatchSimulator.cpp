#include <algorithm>

#include "BatchSimulator.hpp"

template <class TSchedule>
BatchSimulator<TSchedule>::BatchSimulator(std::shared_ptr<Input> input,
					  std::shared_ptr<Scheduler<TSchedule> > scheduler) :
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

template <class TSchedule>
std::vector<Assignation> BatchSimulator<TSchedule>::run()
{
  return {};
}
