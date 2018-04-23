#pragma once

#include <deque>

#include "Input.hpp"
#include "Dispatcher.hpp"

class Simulator
{
 public:
  Simulator(std::shared_ptr<Input> input, std::shared_ptr<Dispatcher> dispatcher);

  void run();

 private:
  std::shared_ptr<Dispatcher> mDispatcher;
  std::deque<std::shared_ptr<Job> > mQueue;
};
