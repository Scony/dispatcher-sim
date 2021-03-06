#pragma once

#include <queue>

#include "Dispatcher.hpp"

class ELOCmp
{
 public:
  ELOCmp(const bool& revparam = false) : mReverse(revparam){};

  bool operator()(const OperationSP& lhs, const OperationSP& rhs) const
  {
    if (mReverse)
      return lt(lhs, rhs);
    return lt(rhs, lhs);
  }

 private:
  bool lt(const OperationSP& lhs, const OperationSP& rhs) const
  {
    if (lhs->arrival == rhs->arrival)
      return lhs->duration > rhs->duration;
    return (lhs->arrival < rhs->arrival);
  }

 private:
  bool mReverse;
};

class ELODispatcher : public Dispatcher
{
 public:
  ELODispatcher(
      std::shared_ptr<Input> input,
      std::shared_ptr<ICloud> cloud,
      std::shared_ptr<IEstimator> estimator);

  OperationSP peek() override;
  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(JobSP job) override;

 protected:
  std::priority_queue<OperationSP, std::vector<OperationSP>, ELOCmp> mOperations;
};
