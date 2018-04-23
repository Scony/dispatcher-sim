#pragma once

#include "Input.hpp"
#include "IScheduler.hpp"

template <class TSchedule>
class BatchSimulator
{
 public:
  BatchSimulator(std::shared_ptr<Input> input, std::shared_ptr<IScheduler<TSchedule> > scheduler);

 private:
  std::shared_ptr<TSchedule> mSchedule;
};
