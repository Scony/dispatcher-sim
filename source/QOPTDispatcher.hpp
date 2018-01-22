#pragma once

#include "Dispatcher.hpp"

class QOPTDispatcher : public Dispatcher
{
public:
  QOPTDispatcher(std::shared_ptr<Input> input,
		 std::shared_ptr<Cloud> cloud,
		 std::shared_ptr<IEstimator> estimator);

  OperationSP pop() override;
  size_t size() override;

  void dispatch(JobSP job) override;

protected:
  std::vector<OperationSP> mQueue;
  long long mTime;
};
