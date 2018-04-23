#pragma once

#include <vector>

#include "IQueue.hpp"

class VectorQueue : public IQueue
{
 public:
  VectorQueue(std::vector<OperationSP>& operations);

  OperationSP peek() override;
  OperationSP pop() override;
  size_t size() override;

 private:
  std::vector<OperationSP>& mOperations;
};
