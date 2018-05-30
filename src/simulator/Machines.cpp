#include <algorithm>

#include "Machines.hpp"
#include "Utility.hpp"

Machines::Machines(std::istream& inputStream)
    : mMachines(Utility::Machines::readFromStream(inputStream))
{
  sortMachinesById();
}

Machines::Machines(unsigned machinesNum, unsigned capacity)
    : mMachines(Utility::Machines::generate(machinesNum, capacity))
{
  sortMachinesById();
}

Machines::Machines(const std::vector<MachineSP>& machines)
    : mMachines(machines)
{
}

size_t Machines::size()
{
  return mMachines.size();
}

std::vector<MachineSP> Machines::fetch()
{
  return mMachines;
}

MachineSP Machines::getMachine(MachineID id)
{
  return mMachines[id];
}

void Machines::sortMachinesById()
{
  std::sort(mMachines.begin(), mMachines.end(), [](MachineSP a, MachineSP b) {
      return a->id < b->id;
    });
  assert(mMachines[0]->id == 0);
}
