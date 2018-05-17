#include <cassert>

#include "DispatcherFactory.hpp"
#include "RandomDispatcher.hpp"
#include "MaxDispatcher.hpp"
#include "MinDispatcher.hpp"
#include "SJLODispatcher.hpp"
#include "FIFODispatcher.hpp"
#include "SJSODispatcher.hpp"
#include "LJSODispatcher.hpp"
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


DispatcherFactory::DispatcherFactory(std::shared_ptr<Input> input,
				     std::shared_ptr<ICloud> cloud,
                                     std::shared_ptr<IEstimator> estimator,
				     Arguments arguments) :
    mInput(input),
    mCloud(cloud),
    mEstimator(estimator),
    mArguments(arguments)
{
}

std::shared_ptr<Dispatcher> DispatcherFactory::create()
{
  std::shared_ptr<Dispatcher> dispatcher;

  if (mArguments.primaryAlgorithm == "random")
    dispatcher = std::make_shared<RandomDispatcher>(mInput, mCloud, mEstimator);
  if (mArguments.primaryAlgorithm == "max")
    dispatcher = std::make_shared<MaxDispatcher>(mInput, mCloud, mEstimator);
  if (mArguments.primaryAlgorithm == "min")
    dispatcher = std::make_shared<MinDispatcher>(mInput, mCloud, mEstimator);
  if (mArguments.primaryAlgorithm == "sjlo")
    dispatcher = std::make_shared<SJLODispatcher>(mInput, mCloud, mEstimator);
  if (mArguments.primaryAlgorithm == "fifo")
    dispatcher = std::make_shared<FIFODispatcher>(mInput, mCloud, mEstimator);
  if (mArguments.primaryAlgorithm == "sjso")
    dispatcher = std::make_shared<SJSODispatcher>(mInput, mCloud, mEstimator);
  if (mArguments.primaryAlgorithm == "ljso")
    dispatcher = std::make_shared<LJSODispatcher>(mInput, mCloud, mEstimator);
  if (mArguments.primaryAlgorithm == "lo")
    dispatcher = std::make_shared<LODispatcher>(mInput, mCloud, mEstimator);
  if (mArguments.primaryAlgorithm == "elo")
    dispatcher = std::make_shared<ELODispatcher>(mInput, mCloud, mEstimator);
  if (mArguments.primaryAlgorithm == "so")
    dispatcher = std::make_shared<SODispatcher>(mInput, mCloud, mEstimator);
  if (mArguments.primaryAlgorithm == "jsa")
    dispatcher = std::make_shared<JSADispatcher>(mInput,
                                                 mCloud,
                                                 mEstimator,
                                                 mArguments.operationLevelAlgorithm,
                                                 mArguments.saIterations);
  if (mArguments.primaryAlgorithm == "qopt")
    dispatcher = std::make_shared<QOPTDispatcher>(mInput, mCloud, mEstimator);
  if (mArguments.primaryAlgorithm == "qworst")
    dispatcher = std::make_shared<QWORSTDispatcher>(mInput, mCloud, mEstimator);
  if (mArguments.primaryAlgorithm == "jopt")
    dispatcher = std::make_shared<JOPTDispatcher>(mInput,
                                                  mCloud,
                                                  mEstimator,
                                                  mArguments.operationLevelAlgorithm);
  if (mArguments.primaryAlgorithm == "sasa")
    dispatcher = std::make_shared<SASADispatcher>(mInput,
                                                  mCloud,
                                                  mEstimator,
                                                  mArguments.operationLevelAlgorithm,
                                                  mArguments.saIterations);
  if (mArguments.primaryAlgorithm == "sjsa")
    dispatcher = std::make_shared<SJSADispatcher>(mInput,
                                                  mCloud,
                                                  mEstimator,
                                                  mArguments.operationLevelAlgorithm,
                                                  mArguments.saIterations);
  if (mArguments.primaryAlgorithm == "sa")
    dispatcher = std::make_shared<SADispatcher>(mInput, mCloud, mEstimator, mArguments.saIterations);
  if (mArguments.primaryAlgorithm == "rvr")
    dispatcher = std::make_shared<VRDispatcher>(mInput, mCloud, mEstimator, false);
  if (mArguments.primaryAlgorithm == "dvr")
    dispatcher = std::make_shared<VRDispatcher>(mInput, mCloud, mEstimator, true);

  assert(dispatcher != nullptr);

  return dispatcher;
}
