#pragma once

#include <vector>
#include <string>

#include "Dispatcher.hpp"
#include "Arguments.hpp"

class DispatcherFactory
{
public:
  DispatcherFactory(std::shared_ptr<Input> input,
		    std::shared_ptr<ICloud> cloud,
		    Arguments arguments);

  std::shared_ptr<Dispatcher> getDispatcher();

private:
  std::shared_ptr<Input> mInput;
  std::shared_ptr<ICloud> mCloud;
  Arguments mArguments;
};
