#include "Utility.hpp"

namespace Utility
{
namespace Machines
{
std::vector<MachineSP> readFromStdin()
{
  std::vector<MachineSP> machines;

  unsigned version;
  std::cin >> version;
  assert(version == 1);

  unsigned machinesNum;
  std::cin >> machinesNum;
  assert(machinesNum > 0);

  for (unsigned i = 0; i < machinesNum; i++)
  {
    long long machineId;
    long long machineCapacity;
    std::cin >> machineId;
    std::cin >> machineCapacity;
    machines.emplace_back(new Machine(machineId, machineCapacity));
  }

  return machines;
}

std::vector<MachineSP> generate(unsigned firstId, unsigned machinesNum, unsigned capacity)
{
  std::vector<MachineSP> machines;
  for (unsigned id = firstId; id < firstId + machinesNum; id++)
    machines.emplace_back(new Machine(id, capacity));
  return machines;
}
}
}
