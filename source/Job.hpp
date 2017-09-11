#pragma once

#include <vector>

#include "Operation.hpp"

struct Job
{
  Job(long long aId,
      long long aPriority,
      long long aArrivalTimestamp,
      std::vector<std::shared_ptr<Operation> > aOperations) :
    id(aId),
    priority(aPriority),
    arrivalTimestamp(aArrivalTimestamp),
    operations(aOperations)
  {
  }

  const long long id;
  const long long priority;
  const long long arrivalTimestamp;
  const std::vector<std::shared_ptr<Operation> > operations;
};
