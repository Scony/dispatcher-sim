#include <iostream>
#include <vector>
#include <cassert>

#include "args.hpp"

#include "Input.hpp"
#include "InputV2.hpp"
#include "ICloud.hpp"
#include "Cloud.hpp"
#include "CloudV2.hpp"
#include "Simulator.hpp"
#include "DispatcherFactory.hpp"
#include "Solution.hpp"
#include "Arguments.hpp"
#include "Utility.hpp"
#include "Scheduler.hpp"
#include "Schedule.hpp"
#include "RandomScheduler.hpp"
#include "SAScheduler.hpp"
#include "BatchSimulator.hpp"

int main(int argc, char ** argv)
{
  srand(time(0));

  args::ArgumentParser parser("Test workload dispatcher simulator");

  args::HelpFlag help(parser, "help", "Display this help and exit",
		      {'h', "help"});

  args::ValueFlag<unsigned> machinesArg(parser, "number", "Number of machines",
					{'m', "machines"});
  args::ValueFlag<unsigned> saIterationsArg(parser, "number", "Number of SA iterations",
                                            {'i', "iterations"});
  args::ValueFlag<std::string> estimationArg(parser, "method", "Estimation method",
					     {'e', "estimation"});
  args::ValueFlag<std::string> opAlgortihmArg(parser, "op_algorithm", "Operation-level algorithm",
					      {'l', "operation-level-algorithm"});
  args::ValueFlag<std::string> outputArg(parser, "type", "Output type",
					 {'o', "output-type"});
  args::ValueFlag<unsigned> setupTimeArg(parser, "number", "Operations setup time",
					 {'s', "setup-time"});
  args::ValueFlag<unsigned> instanceVersionArg(parser, "number", "Instance file version",
                                               {'v', "version"});
  args::ValueFlag<unsigned> kArg(parser, "number", "K-Recent's window size",
				 {'k', "window-size"});
  args::ValueFlag<std::string> representationArg(parser, "representation", "Internal solution representation",
                                                 {'r', "representation"});

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
  arguments.saIterations = saIterationsArg ? args::get(saIterationsArg) : 1;
  arguments.k = kArg ? args::get(kArg) : 3;
  unsigned setupTime = setupTimeArg ? args::get(setupTimeArg) : 0;
  auto outputType = outputArg ? args::get(outputArg) : "jflows";
  auto instanceVersion = instanceVersionArg ? args::get(instanceVersionArg) : 1;
  auto representation = representationArg ? args::get(representationArg) : "queue";

  std::cerr << "reading arguments..." << std::endl;
  std::cerr << "> algorithm: " << arguments.primaryAlgorithm << std::endl;
  std::cerr << "> machines: " << arguments.machinesNum << std::endl;
  std::cerr << "> estimation: " << arguments.estimationMethod << std::endl;
  std::cerr << "> k: " << arguments.k << std::endl;
  std::cerr << "> o-l-algorithm: " << arguments.operationLevelAlgorithm << std::endl;
  std::cerr << "> iterations: " << arguments.saIterations << std::endl;
  std::cerr << "> setup: " << setupTime << std::endl;
  std::cerr << "> output: " << outputType << std::endl;
  std::cerr << "> representation: " << representation << std::endl;
  std::cerr << "reading arguments done" << std::endl;

  std::shared_ptr<Input> input;
  switch (instanceVersion)
  {
    case 2:
      input = std::make_shared<InputV2>();
      break;
    case 1:
    default:
      input = std::make_shared<Input>();
  };

  std::cerr << "reading instance..." << std::endl;
  input->readFromStdin();
  std::cerr << "> jobs: " << input->getJobsNum() << std::endl;
  std::cerr << "> operations: " << input->getOperationsNum() << std::endl;
  std::cerr << "reading instance done" << std::endl;

  auto solutionGatherer = std::make_shared<Solution>();
  std::vector<Assignation> solution;
  std::vector<MachineSP> machines;

  switch (instanceVersion)
  {
    case 2:
      machines = Utility::Machines::readFromStdin();
      break;
    case 1:
    default:
      machines = Utility::Machines::generate(arguments.machinesNum, 1);
      assert(machines.size() == arguments.machinesNum);
  }

  if (representation == "queue")
  {
    std::shared_ptr<ICloud> cloud;
    switch (instanceVersion)
    {
      case 2:
        cloud = std::make_shared<CloudV2>(machines, setupTime);;
        break;
      case 1:
      default:
        cloud = std::make_shared<Cloud>(arguments.machinesNum, setupTime);
    }
    cloud->subscribe(solutionGatherer);

    auto dispatcherFactory = std::make_shared<DispatcherFactory>(input, cloud, arguments);
    auto dispatcher = dispatcherFactory->getDispatcher();

    std::cerr << "running simulation..." << std::endl;
    Simulator simulator(input, dispatcher);
    simulator.run();
    std::cerr << "running simulation done" << std::endl;

    solution = solutionGatherer->getSolutionVec();
  }
  else
  {
    using SchedulerSP = std::shared_ptr<Scheduler<Schedule> >;
    SchedulerSP scheduler = std::make_shared<SAScheduler>(input,
                                                          std::shared_ptr<Machines>(nullptr),
                                                          std::shared_ptr<IEstimator>(nullptr),
                                                          arguments.saIterations);

    std::cerr << "running simulation..." << std::endl;
    BatchSimulator<Schedule> simulator(input, machines, scheduler);
    solution = simulator.run();
    std::cerr << "running simulation done" << std::endl;
  }

  auto jobs = input->getJobs();

  if (outputType == "debug")
  {
    for (const auto& tuple : solution)
      std::cout << "#"
                << std::get<1>(tuple)->id << " ("
                << (std::get<0>(tuple) - std::get<1>(tuple)->duration) << " ; "
                << std::get<0>(tuple) << ") @"
                << std::get<2>(tuple) << std::endl;
  }

  std::cerr << "running validation..." << std::endl;
  assert(Solution::validateOperationEnds(solution));
  assert(Solution::validateSingularOperationExecutions(solution, jobs));
  assert(Solution::validateMachineCapacityUsage(solution, machines));
  std::cerr << "running validation done" << std::endl;

  if (outputType == "jflows")
  {
    auto flowVec = Solution::calculateJobFlowVec(solution, jobs);
    for (const auto& kv : flowVec)
    {
      const auto& flow = kv.first;
      std::cout << flow << std::endl;
    }
  }
  else if (outputType == "opfins")
  {
    for (const auto& tuple : solution)
      std::cout << std::get<0>(tuple) << " "
                << std::get<1>(tuple)->id << " "
                << std::get<2>(tuple) << std::endl;
  }

  return 0;
}
