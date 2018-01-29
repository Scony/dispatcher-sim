#include <iostream>
#include <vector>
#include <cassert>

#include "args.hpp"

#include "Input.hpp"
#include "Cloud.hpp"
#include "Simulator.hpp"
#include "DispatcherFactory.hpp"
#include "Solution.hpp"
#include "Arguments.hpp"

int main(int argc, char ** argv)
{
  srand(time(0));

  args::ArgumentParser parser("Test workload dispatcher simulator");

  args::HelpFlag help(parser, "help", "Display this help and exit",
		      {'h', "help"});

  args::ValueFlag<unsigned> machinesArg(parser, "number", "Number of machines",
					{'m', "machines"});
  args::ValueFlag<std::string> estimationArg(parser, "method", "Estimation method",
					     {'e', "estimation"});
  args::ValueFlag<std::string> opAlgortihmArg(parser, "op_algorithm", "Operation-level algorithm",
					      {"operation-level-algorithm"});

  args::Positional<std::string> algorithmArg(parser, "algorithm", "Primary algorithm");

  try
    {
      parser.ParseCLI(argc, argv);
      if (!algorithmArg)
	throw args::Help("");
    }
  catch (args::Help)
    {
      std::cout << parser;
      return 0;
    }
  catch (args::ParseError e)
    {
      std::cerr << e.what() << std::endl;
      return 1;
    }

  Arguments arguments;
  arguments.primaryAlgorithm = args::get(algorithmArg);
  arguments.machinesNum = machinesArg ? args::get(machinesArg) : 1;
  arguments.estimationMethod = estimationArg ? args::get(estimationArg) : "no";
  arguments.operationLevelAlgorithm = opAlgortihmArg ? args::get(opAlgortihmArg) : "random";

  auto input = std::make_shared<Input>();

  std::cerr << "reading input..." << std::endl;
  input->readFromStdin();
  std::cerr << "> jobs: " << input->getJobsNum() << std::endl;
  std::cerr << "> operations: " << input->getOperationsNum() << std::endl;
  std::cerr << "reading input done" << std::endl;

  auto solution = std::make_shared<Solution>();
  auto cloud = std::make_shared<Cloud>(arguments.machinesNum);

  cloud->subscribe(solution);

  auto dispatcherFactory = std::make_shared<DispatcherFactory>(input, cloud, arguments);
  auto dispatcher = dispatcherFactory->getDispatcher();

  std::cerr << "running simulation..." << std::endl;
  Simulator simulator(input, dispatcher);
  simulator.run();
  std::cerr << "running simulation done" << std::endl;

  auto jobs = input->getJobs();

  std::cerr << "running validation..." << std::endl;
  solution->validate(jobs, arguments.machinesNum);
  std::cerr << "running validation done" << std::endl;

  auto flowVec = solution->calculateJobFlowVec(jobs);
  for (const auto& kv : flowVec)
    {
      const auto& flow = kv.first;
      std::cout << flow << std::endl;
    }

  return 0;
}
