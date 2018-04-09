#pragma once

#include "Operation.hpp"
#include "IQueue.hpp"
#include "ExecutionsSubject.hpp"
#include "IEstimator.hpp"

class ICloud : public ExecutionsSubject
{
public:
  ICloud() : mQueue(nullptr) {}
  virtual ~ICloud() {}

  virtual void advance(long long toTimestamp) = 0;
  virtual std::vector<Assignation> simulate(IEstimatorSP estimator,
					    std::vector<OperationSP> operations) const = 0;
  virtual std::vector<Assignation> simulateWithFuture(IEstimatorSP estimator,
						      std::vector<OperationSP> operations) const = 0;
  void assignQueue(IQueue* queue) { mQueue = queue; }

protected:
  IQueue* mQueue;
};
