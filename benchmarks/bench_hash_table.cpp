#include <benchmark/benchmark.h>

#include <vector>

#include "hash_table.h"

// вставка n ключей
static void BM_HashInsert(benchmark::State& state) {
    const int n = state.range(0);
    for (auto _ : state) {
        HashTable<int, int> t;
        for (int i = 0; i < n; ++i)
            t.insert(i, i);
        benchmark::DoNotOptimize(t);
    }
    state.SetItemsProcessed(state.iterations() * n);
    state.SetComplexityN(n);
}
BENCHMARK(BM_HashInsert)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity();

// цепочки короткие -> O(1), время не растёт с N
static void BM_HashLookup(benchmark::State& state) {
    const int n = state.range(0);
    HashTable<int, int> t;
    for (int i = 0; i < n; ++i)
        t.insert(i, i);
    int i = 0;
    for (auto _ : state) {
        int v = t.at(i % n);
        benchmark::DoNotOptimize(v);
        ++i;
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_HashLookup)->RangeMultiplier(2)->Range(8, 8 << 10);

// искусственные коллизии: все n ключей в одном бакете
static void BM_HashLookupCollisions(benchmark::State& state) {
    const int n = state.range(0);
    HashTable<int, int> t(1024);
    std::vector<int> keys;
    for (int k = 0; static_cast<int>(keys.size()) < n; ++k) {
        if (t.bucketIndexOf(k) == 0)
            keys.push_back(k);
    }
    for (int k : keys)
        t.insert(k, k);
    int target = keys.back();  // самый глубокий элемент цепочки
    for (auto _ : state) {
        int v = t.at(target);
        benchmark::DoNotOptimize(v);
    }
    state.SetComplexityN(n);
}
BENCHMARK(BM_HashLookupCollisions)->RangeMultiplier(2)->Range(8, 64)->Complexity();