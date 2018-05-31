#pragma once

#include "Job.hpp"
#include "Schedule.hpp"
#include "IEstimator.hpp"

namespace schedule
{
namespace algorithm
{
void fifo(Schedule & schedule, JobSP job);
void shortestJob(Schedule & schedule, JobSP job, IEstimatorSP estimator);
void shortestJobLongestOperation(Schedule & schedule, JobSP job, IEstimatorSP estimator);
}
}
