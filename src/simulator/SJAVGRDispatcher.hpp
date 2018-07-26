#pragma once

#include <algorithm>

#include "SJAVGDispatcher.hpp"

class SJAVGRDispatcher : public SJAVGDispatcher
{
 public:
  using SJAVGDispatcher::SJAVGDispatcher;

  void reorderJobOperations(std::vector<OperationSP>& operations) override
  {
    SJAVGDispatcher::reorderJobOperations(operations);
    std::reverse(operations.begin(), operations.end());
  }
};
