#include "IDispatcher.hpp"

IDispatcher::IDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud) :
  mInput(input),
  mCloud(cloud)
{
  mCloud->assignQueue(this);
}

void IDispatcher::advance(long long toTimestamp)
{
  mCloud->advance(toTimestamp);
}
