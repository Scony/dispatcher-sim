#pragma once

#include <map>

#include "QOPTDispatcher.hpp"

class JOPTDispatcher : public QOPTDispatcher
{
public:
  JOPTDispatcher(std::shared_ptr<Input> input,
		 std::shared_ptr<ICloud> cloud,
		 std::shared_ptr<IEstimator> estimator,
		 std::string operationLevelAlgorithm);
};
