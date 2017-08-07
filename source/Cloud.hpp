#pragma once

#include <memory>
#include <deque>

#include "Operation.hpp"

using Queue = std::shared_ptr<std::deque<std::shared_ptr<Operation> > >;

class Cloud
{
public:
  Cloud(int machinesNum) {}

  Queue getQueue() { return mQueue; };

private:
  Queue mQueue;
};
