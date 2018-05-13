#include <iostream>
#include <vector>
#include <cassert>

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
#include "NoEstimator.hpp"
#include "LazyClairvoyantEstimator.hpp"
#include "KRecentEstimator.hpp"
#include "SJLOScheduler.hpp"
#include "TimedScope.hpp"

int main(int argc, char ** argv)
{
  srand(time(0));

  Arguments arguments(argc, argv);

  std::cerr << "Arguments:" << std::endl;
  std::cerr << "> algorithm: " << arguments.primaryAlgorithm << std::endl;
  std::cerr << "> machines: " << arguments.machinesNum << std::endl;
  std::cerr << "> estimation: " << arguments.estimationMethod << std::endl;
  std::cerr << "> k: " << arguments.k << std::endl;
  std::cerr << "> o-l-algorithm: " << arguments.operationLevelAlgorithm << std::endl;
  std::cerr << "> iterations: " << arguments.saIterations << std::endl;
  std::cerr << "> setup: " << arguments.setupTime << std::endl;
  std::cerr << "> output: " << arguments.outputType << std::endl;
  std::cerr << "> representation: " << arguments.representation << std::endl;

  std::shared_ptr<Input> input;
  switch (arguments.instanceVersion)
  {
    case 2:
      input = std::make_shared<InputV2>();
      break;
    case 1:
    default:
      input = std::make_shared<Input>();
  };

  {
    TimedScope ts("reading instance");
    input->readFromStdin();
    std::cerr << "> jobs: " << input->getJobsNum() << std::endl;
    std::cerr << "> operations: " << input->getOperationsNum() << std::endl;
  }

  auto solutionGatherer = std::make_shared<Solution>();
  std::vector<Assignation> solution;
  std::vector<MachineSP> machines;

  switch (arguments.instanceVersion)
  {
    case 2:
      machines = Utility::Machines::readFromStdin();
      break;
    case 1:
    default:
      machines = Utility::Machines::generate(arguments.machinesNum, 1);
      assert(machines.size() == arguments.machinesNum);
  }

  if (arguments.representation == "queue")
  {
    std::shared_ptr<ICloud> cloud;
    switch (arguments.instanceVersion)
    {
      case 2:
        cloud = std::make_shared<CloudV2>(machines, arguments.setupTime);;
        break;
      case 1:
      default:
        cloud = std::make_shared<Cloud>(arguments.machinesNum, arguments.setupTime);
    }
    cloud->subscribe(solutionGatherer);

    auto dispatcherFactory = std::make_shared<DispatcherFactory>(input, cloud, arguments);
    auto dispatcher = dispatcherFactory->getDispatcher();

    Simulator simulator(input, dispatcher);
    {
      TimedScope ts("running simulation");
      simulator.run();
    }
  }
  else if (arguments.representation == "schedule")
  {
    std::shared_ptr<IEstimator> estimator;
    if (arguments.estimationMethod == "no")
      estimator.reset((new NoEstimator()));
    if (arguments.estimationMethod == "lclv")
      estimator.reset((new LazyClairvoyantEstimator()));
    if (arguments.estimationMethod == "krec")
      estimator.reset((new KRecentEstimator(arguments.k)));
    assert(estimator != nullptr);

    using SchedulerSP = std::shared_ptr<Scheduler<Schedule> >;
    SchedulerSP scheduler;
    if (arguments.primaryAlgorithm == "sa")
      scheduler = std::make_shared<SAScheduler>(input,
                                                std::shared_ptr<Machines>(nullptr),
                                                estimator,
                                                arguments.saIterations);
    if (arguments.primaryAlgorithm == "sjlo")
      scheduler = std::make_shared<SJLOScheduler>(input,
                                                  std::shared_ptr<Machines>(nullptr),
                                                  estimator);
    assert(scheduler != nullptr);

    BatchSimulator<Schedule> simulator(input, machines, scheduler);
    simulator.subscribe(estimator);
    simulator.subscribe(solutionGatherer);
    {
      TimedScope ts("running simulation");
      simulator.run();
    }
  }
  else
  {
    throw std::runtime_error("Unrecognized representation");
  }

  solution = solutionGatherer->getSolutionVec();
  auto jobs = input->getJobs();

  if (arguments.outputType == "debug")
  {
    for (const auto& tuple : solution)
      std::cout << "#"
                << std::get<1>(tuple)->id << " ("
                << (std::get<0>(tuple) - std::get<1>(tuple)->duration) << " ; "
                << std::get<0>(tuple) << ") @"
                << std::get<2>(tuple) << std::endl;
  }

  {
    TimedScope ts("running validation");
    assert(Solution::validateOperationEnds(solution));
    assert(Solution::validateSingularOperationExecutions(solution, jobs));
    assert(Solution::validateMachineCapacityUsage(solution, machines));
  }

  if (arguments.outputType == "jflows")
  {
    auto flowVec = Solution::calculateJobFlowVec(solution, jobs);
    for (const auto& kv : flowVec)
    {
      const auto& flow = kv.first;
      std::cout << flow << std::endl;
    }
  }
  else if (arguments.outputType == "opfins")
  {
    for (const auto& tuple : solution)
      std::cout << std::get<0>(tuple) << " "
                << std::get<1>(tuple)->id << " "
                << std::get<2>(tuple) << std::endl;
  }
  else if (arguments.outputType == "jstretches")
  {
    auto stretchVec = Solution::calculateJobStretchVec(solution, jobs);
    for (const auto& kv : stretchVec)
    {
      const auto& stretch = kv.first;
      std::cout << std::fixed << stretch << std::endl;
    }
  }
  else if (arguments.outputType == "nothing")
  {
  }
  else
  {
    throw std::runtime_error("Unrecognized output type");
  }

  return 0;
}
