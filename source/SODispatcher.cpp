#include "SODispatcher.hpp"

SODispatcher::SODispatcher(std::shared_ptr<Input> input,
			   std::shared_ptr<Cloud> cloud,
			   std::shared_ptr<IEstimator> estimator) :
  Dispatcher(input, cloud, estimator)
{
}

void SODispatcher::dispatch(JobSP job)
{
}

OperationSP SODispatcher::pop()
{
}

size_t SODispatcher::size()
{
}
