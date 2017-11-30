#pragma once

#include "Input.hpp"
#include "IDispatcher.hpp"

class Simulator
{
public:
  Simulator(std::shared_ptr<Input> input, std::shared_ptr<IDispatcher> dispatcher);

  void run();

private:
  std::shared_ptr<IDispatcher> mDispatcher;
  std::deque<std::shared_ptr<Job> > mQueue;
};
