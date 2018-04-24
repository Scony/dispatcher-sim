#pragma once

struct Schedule
{
  std::vector<long long> machinesBusySince;
  std::vector<std::vector<OperationSP> > schedule;
};
