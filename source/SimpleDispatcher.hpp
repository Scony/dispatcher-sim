#pragma once

#include "IDispatcher.hpp"

class SimpleDispatcher : public IDispatcher
{
public:
  SimpleDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud);

  void dispatch(std::shared_ptr<Job> job) override;

protected:
  virtual void dispatch(std::shared_ptr<Job> job, Queue queue) = 0;
};
