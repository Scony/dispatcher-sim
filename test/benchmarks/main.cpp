#include <benchmark/benchmark.h>

static void StringCopy(benchmark::State& state)
{
  std::string x = "hello";
  for (auto _ : state)
    std::string copy(x);
}
BENCHMARK(StringCopy);

BENCHMARK_MAIN();
