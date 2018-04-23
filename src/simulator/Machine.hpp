#pragma once

#include <memory>

struct Machine
{
  Machine(long long aId,
	  long long aCapacity):
      id(aId),
      capacity(aCapacity)
  {
  }

  const long long id;
  const long long capacity;
};

using MachineSP = std::shared_ptr<Machine>;
