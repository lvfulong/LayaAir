#include <benchmark/benchmark.h>
#ifdef OS_WINDOWS
#pragma comment ( lib, "Shlwapi.lib" )
#endif
static void BM_SomeFunction(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
      printf("xxxx ");
  }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
BENCHMARK_MAIN();