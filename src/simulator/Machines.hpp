#pragma once

#include <istream>
#include <vector>
#include <unordered_map>

#include "Machine.hpp"

class Machines
{
 public:
  Machines(std::istream& inputStream);
  Machines(unsigned machinesNum, unsigned capacity);
  Machines(const std::vector<MachineSP>& machines);

  size_t size();
  std::vector<MachineSP> fetch();
  MachineSP getMachine(MachineID machineId);

 private:
  void sortMachinesById();

 private:
  std::vector<MachineSP> mMachines;
};
