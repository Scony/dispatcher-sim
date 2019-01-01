#pragma once

#include "IEstimator.hpp"
#include "Job.hpp"
#include "Schedule.hpp"

namespace schedule
{
namespace algorithm
{
void fifo(Schedule& schedule, JobSP job);
void shortestJob(Schedule& schedule, JobSP job, IEstimatorSP estimator);
void shortestJobLongestOperation(Schedule& schedule, JobSP job, IEstimatorSP estimator);
} // namespace algorithm
} // namespace schedule
