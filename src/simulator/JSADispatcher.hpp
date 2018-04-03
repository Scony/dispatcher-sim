#pragma once

#include <map>
#include <string>

#include "Dispatcher.hpp"

class JSADispatcher : public Dispatcher
{
public:
  using Solution = std::vector<long long>;
public:
  JSADispatcher(std::shared_ptr<Input> input,
		std::shared_ptr<Cloud> cloud,
		std::shared_ptr<IEstimator> estimator,
		std::string operationLevelAlgorithm,
		unsigned iterations);

  OperationSP peek() override;
  OperationSP pop() override;
  size_t size() override;

  virtual void dispatch(std::shared_ptr<Job> job) override;

protected:
  const std::string mOperationLevelAlgorithm;
  const unsigned mIterations;

protected:
  Solution mCurrentSolution;
  std::map<long long, std::vector<OperationSP> > mJobOperations;
};
