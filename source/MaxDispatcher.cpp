#include <algorithm>
#include <map>

#include "MaxDispatcher.hpp"

MaxDispatcher::MaxDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud) :
  SimpleDispatcher(input, cloud)
{
}

void MaxDispatcher::dispatch(std::shared_ptr<Job> job,
			     std::vector<std::shared_ptr<Operation> > pendingOperations,
			     Queue queue)
{
  using DoO = std::deque<std::shared_ptr<Operation> >;

  std::map<long long, std::shared_ptr<DoO> > pendingJobOperations;
  for (auto& operation : pendingOperations)
    {
      if (pendingJobOperations.find(operation->id) == pendingJobOperations.end())
	pendingJobOperations[operation->id] = std::make_shared<DoO>();
      pendingJobOperations[operation->id]->push_back(operation);
    }
  for (auto& operation : job->operations)
    {
      if (pendingJobOperations.find(operation->id) == pendingJobOperations.end())
	pendingJobOperations[operation->id] = std::make_shared<DoO>();
      pendingJobOperations[operation->id]->push_back(operation);
    }

  std::vector<std::shared_ptr<DoO> > pendingJobOperationsVec;
  for (auto& kv : pendingJobOperations)
    {
      auto& voo = kv.second;
      pendingJobOperationsVec.push_back(voo);
    }

  std::sort(pendingJobOperationsVec.begin(),
	    pendingJobOperationsVec.end(),
	    [](std::shared_ptr<DoO> a, std::shared_ptr<DoO> b) {
	      return a->at(0)->parentId < b->at(0)->parentId;
	    });

  for (auto& deq : pendingJobOperationsVec)
    std::sort(deq->begin(),
	      deq->end(),
	      [](std::shared_ptr<Operation> a, std::shared_ptr<Operation> b) {
		return a->duration < b->duration; // ASC // TODO: estimation
	      });

  while (true)
    {
      bool added = false;
      for (auto& deq : pendingJobOperationsVec)
	if (deq->size() > 0)
	  {
	    queue->push_front(deq->back());
	    deq->pop_back();
	    added = true;
	  }

      if (!added)
	break;
    }
}
