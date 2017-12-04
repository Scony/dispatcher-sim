#include "Dispatcher.hpp"

Dispatcher::Dispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud) :
  mInput(input),
  mCloud(cloud)
{
  mCloud->assignQueue(this);
}

void Dispatcher::advance(long long toTimestamp)
{
  mCloud->advance(toTimestamp);
}
