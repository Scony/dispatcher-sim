#include "VectorQueue.hpp"

VectorQueue::VectorQueue(std::vector<OperationSP>& operations) :
  mOperations(operations)
{
}

OperationSP VectorQueue::pop()
{
  auto operation = std::move(mOperations.back());
  mOperations.pop_back();
  return operation;
}

size_t VectorQueue::size()
{
  return mOperations.size();
}
