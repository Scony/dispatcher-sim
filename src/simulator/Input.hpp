#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Job.hpp"
#include "Operation.hpp"

class Input
{
 public:
  Input();
  Input(std::vector<std::shared_ptr<Job>> jobs);

  virtual void readFromStdin();

  JobSP getJob(JobID jobId);
  std::vector<std::shared_ptr<Job>> getJobs();
  long long getJobsNum();
  long long getOperationsNum();

 protected:
  std::vector<std::shared_ptr<Job>> mJobs;
  std::unordered_map<JobID, std::shared_ptr<Job>> mJobsMap;
};
