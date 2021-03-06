#pragma once

#include <deque>
#include <map>

#include "Dispatcher.hpp"

class VRDispatcher : public Dispatcher
{
 public:
  VRDispatcher(
      std::shared_ptr<Input> input,
      std::shared_ptr<ICloud> cloud,
      std::shared_ptr<IEstimator> estimator,
      bool deterministic);

  OperationSP peek() override;
  OperationSP pop() override;
  size_t size() override;

  void dispatch(std::shared_ptr<Job> job) override;

 private:
  const bool mDeterministic;
  const unsigned mRules = 4;

  void nextRule();

  std::map<long long, std::deque<OperationSP>> mJobOperations;
  std::vector<std::pair<long long, long long>> mJobsInOrder;
  unsigned mNextJob;
  unsigned mNextRule;
};
