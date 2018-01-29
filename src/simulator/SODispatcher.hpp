#pragma once

#include "Dispatcher.hpp"

class SOCmp
{
public:
  SOCmp(const bool& revparam = false) : mReverse(revparam) {};

  bool operator() (const OperationSP& lhs, const OperationSP& rhs) const
  {
    if (mReverse)
      return lt(lhs, rhs);
    return lt(rhs, lhs);
  }

private:
  bool lt(const OperationSP& lhs, const OperationSP& rhs) const
  {
    if (lhs->duration == rhs->duration)
      return (lhs->arrival < rhs->arrival);
    return lhs->duration < rhs->duration;
  }

private:
  bool mReverse;
};

class SODispatcher : public Dispatcher
{
public:
  SODispatcher(std::shared_ptr<Input> input,
	       std::shared_ptr<Cloud> cloud,
	       std::shared_ptr<IEstimator> estimator);

  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(JobSP job) override;

protected:
  std::priority_queue<OperationSP, std::vector<OperationSP>, SOCmp> mOperations;
};
