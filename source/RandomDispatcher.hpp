#pragma once

#include "SimpleDispatcher.hpp"

class RandomDispatcher : public SimpleDispatcher
{
public:
  RandomDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud);

  void dispatch(std::shared_ptr<Job> job,
		std::vector<std::shared_ptr<Operation> > pendingOperations,
		Queue queue) override;
};
