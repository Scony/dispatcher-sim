#include <climits>

#include <benchmark/benchmark.h>

#include "Cloud.hpp"
#include "VectorQueue.hpp"
#include "NoEstimator.hpp"

static void CloudProcessSmall(benchmark::State& state)
{
  long long fromTimestamp = 0;
  long long toTimestamp = LLONG_MAX;
  long long machinesNum = 1;
  for (auto _ : state)
    {
      auto op1 = std::make_shared<Operation>(1,1,1,1,0,5);
      auto op2 = std::make_shared<Operation>(2,1,1,1,0,5);
      auto op3 = std::make_shared<Operation>(3,1,1,1,0,5);
      std::vector<OperationSP> operations = {op3, op2, op1};
      std::shared_ptr<IQueue> queue = std::make_shared<VectorQueue>(operations);
      std::priority_queue<Machine, std::vector<Machine>, std::greater<Machine> > machines;
      Cloud::process(fromTimestamp,
		     toTimestamp,
		     machinesNum,
		     IEstimatorSP(new NoEstimator),
		     queue.get(),
		     machines);
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
      std::priority_queue<Machine, std::vector<Machine>, std::greater<Machine> > machines;
      Cloud::process(0, LLONG_MAX, 1, IEstimatorSP(new NoEstimator), queue.get(), machines);
    }
}
BENCHMARK(CloudProcessBig);
