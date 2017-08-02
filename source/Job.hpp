#pragma once

struct Job
{
  long long id;
  long long priority;
  long long arrivalTimestamp;
  long long beginTimestamp;
  long long endTimestamp;
  long long duration;
  std::map<long long, Operation> operations;
};
