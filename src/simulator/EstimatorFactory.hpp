#pragma once

#include "Arguments.hpp"
#include "IEstimator.hpp"

class EstimatorFactory
{
 public:
  EstimatorFactory(Arguments);

  IEstimatorSP create();

 private:
  Arguments mArguments;
};
