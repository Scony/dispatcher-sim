#pragma once

#include <vector>
#include <iostream>
#include <cassert>

#include "Machine.hpp"

namespace Utility
{
namespace Machines
{
std::vector<MachineSP> readFromStdin();
std::vector<MachineSP> generate(unsigned machinesNum, unsigned capacity);
}
}
