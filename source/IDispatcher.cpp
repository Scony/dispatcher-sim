#include "IDispatcher.hpp"

void IDispatcher::advance(long long toTimestamp)
{
  mCloud->advance(toTimestamp);
}
