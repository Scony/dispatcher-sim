#pragma once

#include <vector>

#include "Operation.hpp"

struct Job
{
  long long id;
  long long priority;
  long long arrivalTimestamp;
  long long beginTimestamp;
  long long endTimestamp;
  long long duration;
  std::vector<Operation> operations;
};
