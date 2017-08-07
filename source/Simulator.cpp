#include "Simulator.hpp"

Simulator::Simulator(std::shared_ptr<Input> input,
		     std::shared_ptr<Cloud> cloud,
		     std::shared_ptr<IDispatcher> dispatcher) :
  mCloud(cloud),
  mDispatcher(dispatcher),
  mQueue()
{
  // copy input to queue
  // sort queue by arrival time
  // assert order
}

void Simulator::run()
{
  // while there are jobs in queue to dispatch:
  //   get newest job
  //   advance cloud's time
  //   dispatch job
};
