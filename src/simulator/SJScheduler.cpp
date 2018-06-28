#include "SJScheduler.hpp"
#include "ScheduleAlgorithms.hpp"

void SJScheduler::schedule(Schedule & schedule, JobSP job)
{
  schedule::algorithm::shortestJob(schedule, job, mEstimator);
}
