#pragma once

#include <string>

struct Arguments
{
  Arguments(int argc, char** argv);

  std::string primaryAlgorithm;
  unsigned machinesNum;
  std::string estimationMethod;
  std::string operationLevelAlgorithm;
  unsigned saIterations;
  unsigned k;
  unsigned setupTime;
  std::string outputType;
  unsigned instanceVersion;
  std::string representation;
  bool preload;
};
