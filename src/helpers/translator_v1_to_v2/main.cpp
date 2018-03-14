#include <iostream>
#include <map>
#include <vector>

#include "Input.hpp"

int main(int argc, char** argv)
{
  srand(time(0));

  std::map<unsigned, unsigned> capacityToShare = { // TODO: retrieve from file
    { 1, 1600 },
    { 4, 240 },
  };

  std::vector<std::pair<unsigned, unsigned> > capacitiesWCumulatedShares = {{0, 0}};
  for (const auto& kv : capacityToShare)
    capacitiesWCumulatedShares.emplace_back(kv.first,
					    capacitiesWCumulatedShares.back().second + kv.second);

  auto capacity = [&](unsigned x) {
    for (const auto pair : capacitiesWCumulatedShares)
      if (x <= pair.second)
	return (int)pair.first;
    return -1;
  };

  Input input;
  input.readFromStdin();

  std::cout << 0 << std::endl;
  std::cout << 2 << std::endl;

  std::cout << input.getJobsNum() << std::endl;
  for (const auto& job : input.getJobs())
    std::cout << job->id << " "
	      << job->priority << " "
	      << job->arrivalTimestamp
	      << std::endl;

  std::cout << input.getOperationsNum() << std::endl;
  for (const auto& job : input.getJobs())
    for (const auto& operation : job->operations)
      std::cout << job->id << " "
		<< operation->id << " "
		<< operation->name << " "
		<< operation->result << " "
		<< operation->duration << " "
		<< capacity((rand() % capacitiesWCumulatedShares.back().second) + 1)
		<< std::endl;

  return 0;
}
