#include "Utility.hpp"

namespace utility
{
namespace machines
{
std::vector<MachineSP> readFromStream(std::istream& inputStream)
{
  std::vector<MachineSP> machines;

  unsigned version;
  inputStream >> version;
  assert(version == 1);

  unsigned machinesNum;
  inputStream >> machinesNum;
  assert(machinesNum > 0);

  for (unsigned i = 0; i < machinesNum; i++)
  {
    long long machineId;
    long long machineCapacity;
    inputStream >> machineId;
    assert(machineId < machinesNum);
    inputStream >> machineCapacity;
    machines.emplace_back(new Machine(machineId, machineCapacity));
  }

  return machines;
}

std::vector<MachineSP> generate(unsigned machinesNum, unsigned capacity)
{
  const unsigned firstId = 0;
  std::vector<MachineSP> machines;
  for (unsigned id = firstId; id < firstId + machinesNum; id++)
    machines.emplace_back(new Machine(id, capacity));
  return machines;
}
} // namespace machines
} // namespace utility
