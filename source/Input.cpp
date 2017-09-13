#include <iostream>

#include "Input.hpp"

Input::Input() :
  mQueue()
{
  readFromStdin();
}

std::vector<std::shared_ptr<Job> > Input::getJobs()
{
  return mQueue;
}

void Input::readFromStdin()
{
  std::cerr << "reading input..." << std::endl; // TODO: move up

  long long operationsNum = 0;
  int jobsNum;
  std::cin >> jobsNum;
  for (int i = 0; i < jobsNum; i++)
    {
      long long jobId;
      long long jobPriority;
      long long jobArrivalTimestamp;
      int jobOperationsNum;
      std::cin >> jobId;
      std::cin >> jobPriority;
      std::cin >> jobArrivalTimestamp;
      std::cin >> jobOperationsNum;

      std::vector<std::shared_ptr<Operation> > operations;
      for (int j = 0; j < jobOperationsNum; j++)
      	{
	  operationsNum++;
	  long long operationId;
	  long long operationName;
	  long long operationResult;
	  long long operationDuration;
	  std::cin >> operationId;
          std::cin >> operationName;
          std::cin >> operationResult;
      	  std::cin >> operationDuration;
      	  auto operation = std::make_shared<Operation>(operationId,
						       jobId,
						       operationName,
						       operationResult,
						       operationDuration);
      	  operations.push_back(operation);
      	}

      auto job = std::make_shared<Job>(jobId, jobPriority, jobArrivalTimestamp, operations);
      mQueue.push_back(job);
    }

  std::cerr << "reading input done" << std::endl; // 
  std::cerr << "jobs: " << jobsNum << std::endl;  // 
  std::cerr << "operations: " << operationsNum << std::endl; // TODO: move up
}
