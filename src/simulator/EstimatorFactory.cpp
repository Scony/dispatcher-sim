#include <cassert>

#include "EstimatorFactory.hpp"
#include "NoEstimator.hpp"
#include "LazyClairvoyantEstimator.hpp"
#include "KRecentEstimator.hpp"
#include "AverageEstimator.hpp"
#include "ConstEstimator.hpp"
#include "KAverageEstimator.hpp"

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
  if (mArguments.estimationMethod == "avg")
    estimator = std::make_shared<AverageEstimator>();
  if (mArguments.estimationMethod == "const")
    estimator = std::make_shared<ConstEstimator>();
  if (mArguments.estimationMethod == "kavg")
    estimator = std::make_shared<KAverageEstimator>(mArguments.k);

  assert(estimator != nullptr);

  return estimator;
}
