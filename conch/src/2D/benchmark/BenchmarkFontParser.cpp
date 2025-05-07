#include <benchmark/benchmark.h>
#include "../src/font_parser/FontParser.h"
#include "../src/FontDescriptionParser.h"

using namespace laya;

static void BM_FontParserOld(benchmark::State& state) {
    for (auto _ : state) {
        FontDescriptionParser parser;
        FontDescription out;
        parser.parse("20px Arial", out);
    }
}
BENCHMARK(BM_FontParserOld)->Iterations(10000);

static void BM_FontParserNew(benchmark::State& state) {
    for (auto _ : state) {
        bool success;
        FontProperties p = FontParser::parse("20px Arial", &success);
    }
}
BENCHMARK(BM_FontParserNew)->Iterations(10000);

BENCHMARK_MAIN();