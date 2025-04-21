#include <benchmark/benchmark.h>
#include "WebStorage.h"

#ifdef OS_WINDOWS
#pragma comment(lib, "Shlwapi.lib")
#endif

// 基准测试：测试 setItem 方法的性能
static void BM_LocalStorage_SetItem(benchmark::State& state) {
    laya::LocalStorage localStorage;
    localStorage.initialize("localstorage.db");

    for (auto _ : state) {
        localStorage.setItem("key", "value");
    }
}

// 基准测试：测试 getItem 方法的性能
static void BM_LocalStorage_GetItem(benchmark::State& state) {
    laya::LocalStorage localStorage;
    localStorage.initialize("localstorage.db");
    localStorage.setItem("key", "value");

    for (auto _ : state) {
        localStorage.getItem("key");
    }
}

// 基准测试：测试 removeItem 方法的性能
static void BM_LocalStorage_RemoveItem(benchmark::State& state) {
    laya::LocalStorage localStorage;
    localStorage.initialize("localstorage.db");
    localStorage.setItem("key", "value");

    for (auto _ : state) {
        localStorage.removeItem("key");
    }
}

// 注册基准测试
BENCHMARK(BM_LocalStorage_SetItem);
BENCHMARK(BM_LocalStorage_GetItem);
BENCHMARK(BM_LocalStorage_RemoveItem);

// 运行基准测试
BENCHMARK_MAIN();