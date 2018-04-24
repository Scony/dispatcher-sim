#pragma once

#include <deque>

#include "Input.hpp"
#include "Scheduler.hpp"
#include "Types.hpp"

template <class TSchedule>
class BatchSimulator
{
 public:
  BatchSimulator(std::shared_ptr<Input> input, std::shared_ptr<Scheduler<TSchedule> > scheduler);

  std::vector<Assignation> run();

 private:
  std::shared_ptr<Scheduler<TSchedule> > mScheduler;
  std::deque<std::shared_ptr<Job> > mQueue;
  std::shared_ptr<TSchedule> mSchedule;
};
