#include <cassert>

#include "DispatcherFactory.hpp"

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

  assert(dispatcher != nullptr);

  return dispatcher;
}
