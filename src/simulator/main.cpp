#include <iostream>
#include <vector>
#include <cassert>

#include "Input.hpp"
#include "InputV2.hpp"
#include "ICloud.hpp"
#include "Cloud.hpp"
#include "CloudV2.hpp"
#include "Simulator.hpp"
#include "EstimatorFactory.hpp"
#include "DispatcherFactory.hpp"
#include "SchedulerFactory.hpp"
#include "Solution.hpp"
#include "Arguments.hpp"
#include "Machines.hpp"
#include "Scheduler.hpp"
#include "Schedule.hpp"
#include "CapacitySchedule.hpp"
#include "BatchSimulator.hpp"
#include "TimedScope.hpp"

std::shared_ptr<Input> createInput(const Arguments& arguments)
{
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
  return input;
}

std::shared_ptr<Machines> createMachines(const Arguments& arguments)
{
  std::shared_ptr<Machines> machines;
  switch (arguments.instanceVersion)
  {
    case 2:
      machines = std::make_shared<Machines>(std::cin);
      break;
    case 1:
    default:
      machines = std::make_shared<Machines>(arguments.machinesNum, 1);
  }
  return machines;
}

std::shared_ptr<ICloud> createCloud(const Arguments& arguments, std::shared_ptr<Machines> machines)
{
  std::shared_ptr<ICloud> cloud;
  switch (arguments.instanceVersion)
  {
    case 2:
      cloud = std::make_shared<CloudV2>(machines->fetch(), arguments.setupTime);;
      break;
    case 1:
    default:
      cloud = std::make_shared<Cloud>(machines->size(), arguments.setupTime);
  }
  return cloud;
}

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

  std::shared_ptr<Input> input = createInput(arguments);
  {
    TimedScope ts("reading instance");
    input->readFromStdin();
    std::cerr << "> jobs: " << input->getJobsNum() << std::endl;
    std::cerr << "> operations: " << input->getOperationsNum() << std::endl;
  }

  auto solutionGatherer = std::make_shared<Solution>();
  std::vector<Assignation> solution;
  auto machines = createMachines(arguments);
  auto estimatorFactory = std::make_shared<EstimatorFactory>(arguments);
  auto estimator = estimatorFactory->create();

  if (arguments.representation == "queue")
  {
    std::shared_ptr<ICloud> cloud = createCloud(arguments, machines);
    cloud->subscribe(solutionGatherer);
    cloud->subscribe(estimator);

    auto dispatcherFactory = std::make_shared<DispatcherFactory>(input, cloud, estimator, arguments);
    auto dispatcher = dispatcherFactory->create();

    Simulator simulator(input, dispatcher);
    {
      TimedScope ts("running simulation");
      simulator.run();
    }
  }
  else if (arguments.representation == "schedule")
  {
    if (arguments.instanceVersion == 1)
    {
      auto schedulerFactory =
          std::make_shared<SchedulerFactory<Schedule> >(input, machines, estimator, arguments);
      auto scheduler = schedulerFactory->create();
      BatchSimulator<Schedule> simulator(input, machines, scheduler);
      simulator.subscribe(estimator);
      simulator.subscribe(solutionGatherer);
      {
        TimedScope ts("running simulation");
        simulator.run();
      }
    } else
    {
      auto schedulerFactory =
          std::make_shared<SchedulerFactory<CapacitySchedule> >(input, machines, estimator, arguments);
      auto scheduler = schedulerFactory->create();
      BatchSimulator<CapacitySchedule> simulator(input, machines, scheduler);
      simulator.subscribe(estimator);
      simulator.subscribe(solutionGatherer);
      {
        TimedScope ts("running simulation");
        simulator.run();
      }
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
    assert(Solution::validateMachineCapacityUsage(solution, machines->fetch()));
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
  else if (arguments.outputType == "nothing" or arguments.outputType == "debug")
  {
  }
  else
  {
    throw std::runtime_error("Unrecognized output type");
  }

  return 0;
}
