#pragma once

#include "Input.hpp"
#include "Cloud.hpp"
#include "IDispatcher.hpp"

class Simulator
{
public:
  Simulator(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud, std::shared_ptr<IDispatcher> dispatcher) {}

  void run() {};
};
