#pragma once

#include <algorithm>

#include "SJMDDispatcher.hpp"

class SJMDRDispatcher : public SJMDDispatcher
{
 public:
  using SJMDDispatcher::SJMDDispatcher;

  void reorderJobOperations(std::vector<OperationSP>& operations) override
  {
    SJMDDispatcher::reorderJobOperations(operations);
    std::reverse(operations.begin(), operations.end());
  }
};
