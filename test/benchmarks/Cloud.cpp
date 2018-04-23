#include <climits>

#include <benchmark/benchmark.h>

#include "Cloud.hpp"
#include "VectorQueue.hpp"
#include "NoEstimator.hpp"

static void CloudProcessSmall(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::vector<OperationSP> operations;
    for (int i = 1; i < 3; i++)
      operations.push_back(std::make_shared<Operation>(i,1,1,1,0,5));
    std::shared_ptr<IQueue> queue = std::make_shared<VectorQueue>(operations);
    Cloud::FreeMachines freeMachines = {{1, -1}};
    Cloud::BusyMachines busyMachines;
    long long seq = 0;
    Cloud::process(0,
                   LLONG_MAX,
                   1,
                   IEstimatorSP(new NoEstimator),
                   queue.get(),
                   freeMachines,
                   busyMachines,
                   seq);
  }
}
BENCHMARK(CloudProcessSmall);

static void CloudProcessBig(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::vector<OperationSP> operations;
    for (int i = 1; i < 1024; i++)
      operations.push_back(std::make_shared<Operation>(i,1,1,1,0,5));
    std::shared_ptr<IQueue> queue = std::make_shared<VectorQueue>(operations);
    Cloud::FreeMachines freeMachines = {{1, -1}};
    Cloud::BusyMachines busyMachines;
    long long seq = 0;
    Cloud::process(0,
                   LLONG_MAX,
                   1,
                   IEstimatorSP(new NoEstimator),
                   queue.get(),
                   freeMachines,
                   busyMachines,
                   seq);
  }
}
BENCHMARK(CloudProcessBig);
