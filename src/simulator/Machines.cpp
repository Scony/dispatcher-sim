#include "Machines.hpp"
#include "Utility.hpp"

Machines::Machines(std::istream& inputStream)
    : mMachines(Utility::Machines::readFromStream(inputStream))
{
}

Machines::Machines(unsigned machinesNum, unsigned capacity)
    : mMachines(Utility::Machines::generate(machinesNum, capacity))
{
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
