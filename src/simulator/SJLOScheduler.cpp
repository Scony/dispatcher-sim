#include "SJLOScheduler.hpp"
#include "ScheduleAlgorithms.hpp"

SJLOScheduler::SJLOScheduler(
    std::shared_ptr<Input> input,
    std::shared_ptr<Machines> machines,
    std::shared_ptr<IEstimator> estimator)
    : Scheduler(input, machines, estimator)
{
}

void SJLOScheduler::schedule(Schedule& schedule, JobSP job)
{
  schedule::algorithm::shortestJobLongestOperation(schedule, job, mEstimator);
}
