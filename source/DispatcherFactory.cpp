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

DispatcherFactory::DispatcherFactory(std::shared_ptr<Input> input,
				     std::shared_ptr<Cloud> cloud,
				     std::vector<std::string> args) :
  mInput(input),
  mCloud(cloud),
  mArgs(args)
{
}

std::shared_ptr<Dispatcher> DispatcherFactory::getDispatcher()
{
  assert(mArgs.size() >= 2);

  // estimator

  std::shared_ptr<IEstimator> estimator;

  if (mArgs[1] == "no")
    estimator.reset((new NoEstimator()));
  if (mArgs[1] == "lclv")
    estimator.reset((new LazyClairvoyantEstimator()));

  assert(estimator != nullptr);

  // dispatcher

  std::shared_ptr<Dispatcher> dispatcher;

  if (mArgs[0] == "random")
    dispatcher.reset((new RandomDispatcher(mInput, mCloud, estimator)));
  if (mArgs[0] == "max")
    dispatcher.reset((new MaxDispatcher(mInput, mCloud, estimator)));
  if (mArgs[0] == "min")
    dispatcher.reset((new MinDispatcher(mInput, mCloud, estimator)));
  if (mArgs[0] == "sjlo")
    dispatcher.reset((new SJLODispatcher(mInput, mCloud, estimator)));
  if (mArgs[0] == "fifo")
    dispatcher.reset((new FIFODispatcher(mInput, mCloud, estimator)));
  if (mArgs[0] == "sjso")
    dispatcher.reset((new SJSODispatcher(mInput, mCloud, estimator)));
  if (mArgs[0] == "ljso")
    dispatcher.reset((new LJSODispatcher(mInput, mCloud, estimator)));
  if (mArgs[0] == "lo")
    dispatcher.reset((new LODispatcher(mInput, mCloud, estimator)));
  if (mArgs[0] == "so")
    dispatcher.reset((new SODispatcher(mInput, mCloud, estimator)));

  assert(dispatcher != nullptr);

  return dispatcher;
}
