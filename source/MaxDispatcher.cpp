#include <algorithm>
#include <map>

#include "MaxDispatcher.hpp"

MaxDispatcher::MaxDispatcher(std::shared_ptr<Input> input, std::shared_ptr<Cloud> cloud) :
  SimpleDispatcher(input, cloud)
{
}

void MaxDispatcher::dispatch(std::shared_ptr<Job> job, Queue queue)
{
  using DoO = std::deque<std::shared_ptr<Operation> >;

  std::vector<std::shared_ptr<Operation> > pendingOperations;
  pendingOperations.insert(pendingOperations.end(), job->operations.begin(), job->operations.end());
  pendingOperations.insert(pendingOperations.end(), queue->begin(), queue->end());
  queue->clear();

  std::map<long long, std::shared_ptr<DoO> > orderedJobOperations;
  for (auto& operation : pendingOperations)
    {
      if (orderedJobOperations.find(operation->parentId) == orderedJobOperations.end())
	orderedJobOperations[operation->parentId] = std::make_shared<DoO>();
      orderedJobOperations[operation->parentId]->push_back(operation);
    }

  std::vector<std::shared_ptr<DoO> > orderedJobOperationsVec;
  for (auto& kv : orderedJobOperations)
    {
      auto& voo = kv.second;
      orderedJobOperationsVec.push_back(voo);
    }

  // assure deterministic processing (sort by job's ID)
  std::sort(orderedJobOperationsVec.begin(),
	    orderedJobOperationsVec.end(),
	    [](std::shared_ptr<DoO> a, std::shared_ptr<DoO> b) {
	      return a->at(0)->parentId < b->at(0)->parentId;
	    });

  // assure order of operations within job
  for (auto& jobOperations : orderedJobOperationsVec)
    std::sort(jobOperations->begin(),
	      jobOperations->end(),
	      [](std::shared_ptr<Operation> a, std::shared_ptr<Operation> b) {
		return a->duration < b->duration; // ASC // TODO: estimation
	      });

  while (true)
    {
      bool added = false;
      for (auto& deq : orderedJobOperationsVec)
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
