#include <algorithm>
#include <climits>
#include <iostream>

#include "Simulator.hpp"

Simulator::Simulator(std::shared_ptr<Input> input, std::shared_ptr<Dispatcher> dispatcher) :
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
    auto arrivalTimestamp = mQueue.back()->arrivalTimestamp;
    mDispatcher->advance(arrivalTimestamp);
    std::vector<JobSP> jobsToDispatch;
    while (mQueue.size() > 0 && mQueue.back()->arrivalTimestamp == arrivalTimestamp)
    {
      jobsToDispatch.push_back(mQueue.back());
      mQueue.pop_back();
    }
    std::cerr << "dispatching..." << std::endl;
    mDispatcher->dispatch(jobsToDispatch);
  }

  mDispatcher->advance(LLONG_MAX);
};
