#include <cassert>

#include "DispatcherFactory.hpp"
#include "RandomDispatcher.hpp"
#include "MaxDispatcher.hpp"
#include "MinDispatcher.hpp"
#include "NoEstimator.hpp"
#include "SJLODispatcher.hpp"
#include "FIFODispatcher.hpp"
#include "SJSODispatcher.hpp"
#include "LJSODispatcher.hpp"
#include "LazyClairvoyantEstimator.hpp"
#include "LODispatcher.hpp"
#include "SODispatcher.hpp"
#include "ELODispatcher.hpp"
#include "JSADispatcher.hpp"
#include "QOPTDispatcher.hpp"
#include "QWORSTDispatcher.hpp"
#include "JOPTDispatcher.hpp"
#include "SASADispatcher.hpp"
#include "SJSADispatcher.hpp"
#include "SADispatcher.hpp"
#include "VRDispatcher.hpp"
#include "KRecentEstimator.hpp"

DispatcherFactory::DispatcherFactory(std::shared_ptr<Input> input,
				     std::shared_ptr<Cloud> cloud,
				     Arguments arguments) :
  mInput(input),
  mCloud(cloud),
  mArguments(arguments)
{
}

std::shared_ptr<Dispatcher> DispatcherFactory::getDispatcher()
{
  // estimator

  std::shared_ptr<IEstimator> estimator;

  if (mArguments.estimationMethod == "no")
    estimator.reset((new NoEstimator()));
  if (mArguments.estimationMethod == "lclv")
    estimator.reset((new LazyClairvoyantEstimator()));
  if (mArguments.estimationMethod == "krec")
    estimator.reset((new KRecentEstimator(mArguments.k)));

  assert(estimator != nullptr);

  // dispatcher

  std::shared_ptr<Dispatcher> dispatcher;

  if (mArguments.primaryAlgorithm == "random")
    dispatcher.reset((new RandomDispatcher(mInput, mCloud, estimator)));
  if (mArguments.primaryAlgorithm == "max")
    dispatcher.reset((new MaxDispatcher(mInput, mCloud, estimator)));
  if (mArguments.primaryAlgorithm == "min")
    dispatcher.reset((new MinDispatcher(mInput, mCloud, estimator)));
  if (mArguments.primaryAlgorithm == "sjlo")
    dispatcher.reset((new SJLODispatcher(mInput, mCloud, estimator)));
  if (mArguments.primaryAlgorithm == "fifo")
    dispatcher.reset((new FIFODispatcher(mInput, mCloud, estimator)));
  if (mArguments.primaryAlgorithm == "sjso")
    dispatcher.reset((new SJSODispatcher(mInput, mCloud, estimator)));
  if (mArguments.primaryAlgorithm == "ljso")
    dispatcher.reset((new LJSODispatcher(mInput, mCloud, estimator)));
  if (mArguments.primaryAlgorithm == "lo")
    dispatcher.reset((new LODispatcher(mInput, mCloud, estimator)));
  if (mArguments.primaryAlgorithm == "elo")
    dispatcher.reset((new ELODispatcher(mInput, mCloud, estimator)));
  if (mArguments.primaryAlgorithm == "so")
    dispatcher.reset((new SODispatcher(mInput, mCloud, estimator)));
  if (mArguments.primaryAlgorithm == "jsa")
    dispatcher.reset((new JSADispatcher(mInput,
					mCloud,
					estimator,
					mArguments.operationLevelAlgorithm,
					mArguments.saIterations)));
  if (mArguments.primaryAlgorithm == "qopt")
    dispatcher.reset((new QOPTDispatcher(mInput, mCloud, estimator)));
  if (mArguments.primaryAlgorithm == "qworst")
    dispatcher.reset((new QWORSTDispatcher(mInput, mCloud, estimator)));
  if (mArguments.primaryAlgorithm == "jopt")
    dispatcher.reset((new JOPTDispatcher(mInput,
					 mCloud,
					 estimator,
					 mArguments.operationLevelAlgorithm)));
  if (mArguments.primaryAlgorithm == "sasa")
    dispatcher.reset((new SASADispatcher(mInput,
					 mCloud,
					 estimator,
					 mArguments.operationLevelAlgorithm,
					 mArguments.saIterations)));
  if (mArguments.primaryAlgorithm == "sjsa")
    dispatcher.reset((new SJSADispatcher(mInput,
					 mCloud,
					 estimator,
					 mArguments.operationLevelAlgorithm,
					 mArguments.saIterations)));
  if (mArguments.primaryAlgorithm == "sa")
    dispatcher.reset((new SADispatcher(mInput, mCloud, estimator, mArguments.saIterations)));
  if (mArguments.primaryAlgorithm == "rvr")
    dispatcher.reset((new VRDispatcher(mInput, mCloud, estimator, false)));
  if (mArguments.primaryAlgorithm == "dvr")
    dispatcher.reset((new VRDispatcher(mInput, mCloud, estimator, true)));

  assert(dispatcher != nullptr);

  return dispatcher;
}
