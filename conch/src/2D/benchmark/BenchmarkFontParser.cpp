#include <benchmark/benchmark.h>
#include "../src/font_parser/FontParser.h"
#include "../tests/FontDescriptionParser.h"
#include <utils/LRUCache.h>

using namespace laya;

static void BM_FontParserOld(benchmark::State& state) {
    int i = 0;
    for (auto _ : state) {
        FontDescriptionParser parser;
        FontDescription out;
        auto font = std::to_string(i % 256) + "px Arial";
        i++;
        parser.parse(font, out);
    }
}
BENCHMARK(BM_FontParserOld)->Iterations(10000);

static void BM_FontParserNew(benchmark::State& state) {
    int i = 0;
    for (auto _ : state) {
        bool success;
        auto font = std::to_string(i % 256) + "px Arial";
        i++;
        FontProperties p = FontParser::parse(font, &success);
    }
}
BENCHMARK(BM_FontParserNew)->Iterations(10000);

static LRUCache<std::string, FontProperties> cache(256);

static void BM_FontParserNewWithCache(benchmark::State& state) {
    int i = 0;
    bool success;
    for (auto _ : state) {
        auto font = std::to_string(i % 256) + "px Arial";
        i++;
        auto p = cache.get(font);
        if (!p) {
            cache.put(font, FontParser::parse(font, &success));
        }
    }
}
BENCHMARK(BM_FontParserNewWithCache)->Iterations(10000);
BENCHMARK_MAIN();