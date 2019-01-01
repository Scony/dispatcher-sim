#pragma once

#include <istream>
#include <unordered_map>
#include <vector>

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
