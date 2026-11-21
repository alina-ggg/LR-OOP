#include <benchmark/benchmark.h>

#include "array.h"
#include <benchmark/benchmark.h>

#include "array.h"

// 1. pushBack с ростом буфера: суммарно O(N) -> амортизированная O(1) на
// элемент
static void BM_PushBack(benchmark::State& state) {
    const int n = state.range(0);
    for (auto _ : state) {
        Array<int> arr;
        for (int i = 0; i < n; ++i) {
            arr.pushBack(i);
        }
        benchmark::DoNotOptimize(arr);  // запрещаем оптимизатору выкинуть код
    }
    state.SetItemsProcessed(state.iterations() * n);
    state.SetComplexityN(n);
}
BENCHMARK(BM_PushBack)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity();

// 2. pushBack с заранее выделенной вместимостью (без перевыделений)
static void BM_PushBackReserved(benchmark::State& state) {
    const int n = state.range(0);
    for (auto _ : state) {
        Array<int> arr(static_cast<std::size_t>(n));
        for (int i = 0; i < n; ++i) {
            arr.pushBack(i);
        }
        benchmark::DoNotOptimize(arr);
    }
    state.SetItemsProcessed(state.iterations() * n);
    state.SetComplexityN(n);
}
BENCHMARK(BM_PushBackReserved)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity();

// 3. вставка в начало: O(n) на операцию из-за сдвига всех элементов
static void BM_InsertAtBegin(benchmark::State& state) {
    const int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();  // подготовку не замеряем
        Array<int> arr(static_cast<std::size_t>(n));
        state.ResumeTiming();
        for (int i = 0; i < n; ++i) {
            arr.insertAt(0, i);
        }
        benchmark::DoNotOptimize(arr);
    }
    state.SetComplexityN(n);
}
BENCHMARK(BM_InsertAtBegin)->RangeMultiplier(2)->Range(8, 4 << 10)->Complexity();

// 4. удаление из начала: тоже O(n) на операцию
static void BM_RemoveAtBegin(benchmark::State& state) {
    const int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        Array<int> arr(static_cast<std::size_t>(n));
        for (int i = 0; i < n; ++i)
            arr.pushBack(i);
        state.ResumeTiming();
        for (int i = 0; i < n; ++i) {
            arr.removeAt(0);
        }
        benchmark::DoNotOptimize(arr);
    }
    state.SetComplexityN(n);
}
BENCHMARK(BM_RemoveAtBegin)->RangeMultiplier(2)->Range(8, 4 << 10)->Complexity();

// 5. доступ по индексу: O(1) — время не зависит от N
static void BM_Access(benchmark::State& state) {
    const int n = state.range(0);
    Array<int> arr(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i)
        arr.pushBack(i);
    int i = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(arr[i % n]);
        ++i;
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Access)->RangeMultiplier(2)->Range(8, 8 << 10);