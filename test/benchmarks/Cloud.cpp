#include <benchmark/benchmark.h>

static void StringCopy2(benchmark::State& state)
{
  std::string x = "hello";
  for (auto _ : state)
    std::string copy(x);
}
BENCHMARK(StringCopy2);
