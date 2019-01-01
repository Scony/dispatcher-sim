#include "Dispatcher.hpp"

Dispatcher::Dispatcher(
    std::shared_ptr<Input> input,
    std::shared_ptr<ICloud> cloud,
    std::shared_ptr<IEstimator> estimator)
    : mInput(input), mCloud(cloud), mEstimator(estimator)
{
  mCloud->assignQueue(this);
}

void Dispatcher::advance(long long toTimestamp)
{
  mCloud->advance(toTimestamp);
}
