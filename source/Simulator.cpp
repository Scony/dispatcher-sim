#include <algorithm>
#include <climits>

#include "Simulator.hpp"

Simulator::Simulator(std::shared_ptr<Input> input, std::shared_ptr<IDispatcher> dispatcher) :
  mDispatcher(dispatcher),
  mQueue()
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

void Simulator::run()
{
  while (mQueue.size() > 0)
    {
      auto newestJob = mQueue.back();
      mDispatcher->advance(newestJob->arrivalTimestamp);
      mDispatcher->dispatch(newestJob);
      mQueue.pop_back();
    }

  mDispatcher->advance(LLONG_MAX);
};
