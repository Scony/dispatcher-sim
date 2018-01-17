#pragma once

#include "Dispatcher.hpp"
#include "Solution.hpp"

class JobSADispatcher : public Dispatcher
{
public:
  JobSADispatcher(std::shared_ptr<Input> input,
		  std::shared_ptr<Cloud> cloud,
		  std::shared_ptr<IEstimator> estimator);

  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(std::shared_ptr<Job> job) override;

private:
  std::vector<OperationSP> mQueue;
  std::shared_ptr<Solution> mSolution;
};
