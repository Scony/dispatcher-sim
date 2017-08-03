#include <iostream>

#include "Input.hpp"

Input::Input() :
  mQueue()
{
}

void Input::readFromStdin()
{
  int jobsNum;
  std::cin >> jobsNum;
  for (int i = 0; i < jobsNum; i++)
    {
      auto job = std::make_shared<Job>();
      std::cin >> job->id;
      std::cin >> job->priority;
      std::cin >> job->arrivalTimestamp;
      int operationsNum;
      std::cin >> operationsNum;
      for (int j = 0; j < operationsNum; j++)
      	{
      	  auto operation = std::make_shared<Operation>();
          operation->parentId = job->id;
	  std::cin >> operation->id;
          std::cin >> operation->name;
          std::cin >> operation->result;
      	  std::cin >> operation->duration;
      	  job->operations.push_back(operation);
      	}
      mQueue.push_back(job);
    }
}
