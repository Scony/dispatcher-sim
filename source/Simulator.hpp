#pragma once

#include "Input.hpp"
#include "Cloud.hpp"

class Simulator
{
public:
  Simulator(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud) {}

  void run() {};
};
