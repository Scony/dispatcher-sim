#pragma once

#include "IEstimator.hpp"
#include "Arguments.hpp"

class EstimatorFactory
{
 public:
  EstimatorFactory(Arguments);

  IEstimatorSP create();

 private:
  Arguments mArguments;
};
