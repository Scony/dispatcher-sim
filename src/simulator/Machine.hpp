#pragma once

#include <memory>

#include "Types.hpp"

struct Machine
{
  Machine(MachineID aId,
	  long long aCapacity):
      id(aId),
      capacity(aCapacity)
  {
  }

  const MachineID id;
  const long long capacity;
};

using MachineSP = std::shared_ptr<Machine>;
