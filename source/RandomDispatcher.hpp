#pragma once

#include "IDispatcher.hpp"

class RandomDispatcher : public IDispatcher
{
public:
  RandomDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud);

  OperationSP pop() override;
  size_t size() override;

  void dispatch(JobSP job) override;

private:
  std::vector<OperationSP> mQueue;
};
