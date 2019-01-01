#pragma once

#include <string>
#include <vector>

#include "Arguments.hpp"
#include "Dispatcher.hpp"
#include "IEstimator.hpp"

class DispatcherFactory
{
 public:
  DispatcherFactory(
      std::shared_ptr<Input> input,
      std::shared_ptr<ICloud> cloud,
      std::shared_ptr<IEstimator> estimator,
      Arguments arguments);

  std::shared_ptr<Dispatcher> create();

 private:
  std::shared_ptr<Input> mInput;
  std::shared_ptr<ICloud> mCloud;
  std::shared_ptr<IEstimator> mEstimator;
  Arguments mArguments;
};
