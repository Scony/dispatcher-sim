#pragma once

namespace Schedule
{
struct Schedule
{
  std::vector<long long> machineReadyTimes;
  std::vector<std::vector<OperationSP> > schedule;
};
}
