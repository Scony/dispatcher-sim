#pragma once

#include "Job.hpp"
#include "Schedule.hpp"

namespace ScheduleAlgorithms
{
void fifo(Schedule & schedule, JobSP job);
void shortest_job(Schedule & schedule, JobSP job);
}
