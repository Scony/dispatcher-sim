#pragma once

#include "Dispatcher.hpp"

class QOPTDispatcher : public Dispatcher
{
public:
  QOPTDispatcher(std::shared_ptr<Input> input,
		 std::shared_ptr<Cloud> cloud,
		 std::shared_ptr<IEstimator> estimator,
		 bool calculateSolution = true);

  virtual OperationSP peek() override;
  virtual OperationSP pop() override;
  virtual size_t size() override;

  virtual void dispatch(JobSP job) override;

protected:
  std::vector<OperationSP> mQueue;
  long long mTime;
};
