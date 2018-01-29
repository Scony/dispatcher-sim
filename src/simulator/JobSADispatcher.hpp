#pragma once

#include <map>

#include "Dispatcher.hpp"

class JobSADispatcher : public Dispatcher
{
public:
  using Solution = std::vector<long long>;
public:
  JobSADispatcher(std::shared_ptr<Input> input,
		  std::shared_ptr<Cloud> cloud,
		  std::shared_ptr<IEstimator> estimator);

  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(std::shared_ptr<Job> job) override;

private:
  Solution mCurrentSolution;
  std::map<long long, std::vector<OperationSP> > mJobOperations;
};
