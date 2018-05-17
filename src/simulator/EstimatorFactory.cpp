#include <cassert>

#include "EstimatorFactory.hpp"
#include "NoEstimator.hpp"
#include "LazyClairvoyantEstimator.hpp"
#include "KRecentEstimator.hpp"

EstimatorFactory::EstimatorFactory(Arguments arguments) :
    mArguments(arguments)
{
}

IEstimatorSP EstimatorFactory::create()
{
  std::shared_ptr<IEstimator> estimator;

  if (mArguments.estimationMethod == "no")
    estimator = std::make_shared<NoEstimator>();
  if (mArguments.estimationMethod == "lclv")
    estimator = std::make_shared<LazyClairvoyantEstimator>();
  if (mArguments.estimationMethod == "krec")
    estimator = std::make_shared<KRecentEstimator>(mArguments.k);

  assert(estimator != nullptr);

  return estimator;
}
