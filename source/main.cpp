#include <iostream>
#include <vector>
#include <cassert>

#include "Input.hpp"
#include "Cloud.hpp"
#include "Simulator.hpp"
#include "DispatcherFactory.hpp"
#include "Solution.hpp"

int main(int argc, char ** argv)
{
  srand(time(0));

  std::vector<std::string> args;
  for (int i = 0; i < argc; i++)
    args.push_back(argv[i]);

  assert(argc >= 2 + 1);
  unsigned machinesNum = std::stoi(args[1]);

  auto input = std::make_shared<Input>();
  auto solution = std::make_shared<Solution::Solution>();
  auto cloud = std::make_shared<Cloud>(machinesNum, solution);

  auto factoryArgs = std::vector<std::string>(args.begin() + 2, args.end());
  auto dispatcherFactory = std::make_shared<DispatcherFactory>(input, cloud, factoryArgs);
  auto dispatcher = dispatcherFactory->getDispatcher();

  std::cerr << "reading input..." << std::endl;
  input->readFromStdin();
  std::cerr << "> jobs: " << input->getJobsNum() << std::endl;
  std::cerr << "> operations: " << input->getOperationsNum() << std::endl;
  std::cerr << "reading input done" << std::endl;

  std::cerr << "running simulation..." << std::endl;
  Simulator simulator(input, cloud, dispatcher);
  simulator.run();
  std::cerr << "running simulation done" << std::endl;

  auto jobs = input->getJobs();

  std::cerr << "running validation..." << std::endl;
  Solution::validate(jobs, solution, machinesNum);
  std::cerr << "running validation done" << std::endl;

  auto flowVec = Solution::calculateFlow(jobs, solution);
  for (const auto& kv : flowVec)
    {
      const auto& flow = kv.first;
      std::cout << flow << std::endl;
    }

  return 0;
}
