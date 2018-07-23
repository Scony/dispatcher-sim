#pragma once

#include <algorithm>

#include "SJDispatcher.hpp"

class SJRANDDispatcher : public SJDispatcher
{
 public:
  using SJDispatcher::SJDispatcher;

  void reorderJobOperations(std::vector<OperationSP>& operations)
  {
    std::random_shuffle(operations.begin(), operations.end());
  }
};
