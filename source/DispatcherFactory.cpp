#include <cassert>

#include "DispatcherFactory.hpp"
#include "RandomDispatcher.hpp"
#include "MaxDispatcher.hpp"
#include "MinDispatcher.hpp"

DispatcherFactory::DispatcherFactory(std::shared_ptr<Input> input,
				     std::shared_ptr<Cloud> cloud,
				     std::vector<std::string> args) :
  mInput(input),
  mCloud(cloud),
  mArgs(args)
{
}

std::shared_ptr<IDispatcher> DispatcherFactory::getDispatcher()
{
  assert(mArgs.size() >= 1);

  std::shared_ptr<IDispatcher> dispatcher;

  if (mArgs[0] == "random")
    dispatcher.reset((new RandomDispatcher(mInput, mCloud)));
  if (mArgs[0] == "max")
    dispatcher.reset((new MaxDispatcher(mInput, mCloud)));
  if (mArgs[0] == "min")
    dispatcher.reset((new MinDispatcher(mInput, mCloud)));

  assert(dispatcher != nullptr);

  return dispatcher;
}
