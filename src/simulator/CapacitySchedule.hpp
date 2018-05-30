#pragma once

#include <vector>

#include "Operation.hpp"
#include "Types.hpp"
#include "IEstimator.hpp"
#include "Machines.hpp"

struct CapacitySchedule
{
  CapacitySchedule(std::shared_ptr<Machines> machines);

  std::vector<Assignation> dispatch(long long from, long long until);
  std::vector<Assignation> simulateDispatch(long long from, IEstimatorSP estimator) const;

  using BeginTimestamp = long long;
  std::vector<std::vector<std::pair<BeginTimestamp, OperationSP> > > ongoings;
  std::vector<std::vector<OperationSP> > schedule;
  std::shared_ptr<Machines> machines;
};
