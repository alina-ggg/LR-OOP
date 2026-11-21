#include <benchmark/benchmark.h>

#include "queue.h"
#include "stack.h"

static void BM_StackPush(benchmark::State& state) {
    const int n = state.range(0);
    for (auto _ : state) {
        Stack<int> st;
        for (int i = 0; i < n; ++i)
            st.push(i);
        benchmark::DoNotOptimize(st);
    }
    state.SetItemsProcessed(state.iterations() * n);
    state.SetComplexityN(n);
}
BENCHMARK(BM_StackPush)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity();

// полный цикл LIFO: n push + n pop -> O(N) суммарно, O(1) на операцию
static void BM_StackPushPop(benchmark::State& state) {
    const int n = state.range(0);
    for (auto _ : state) {
        Stack<int> st;
        for (int i = 0; i < n; ++i)
            st.push(i);
        for (int i = 0; i < n; ++i) {
            int v = st.pop();
            benchmark::DoNotOptimize(v);
        }
    }
    state.SetItemsProcessed(state.iterations() * 2 * n);
    state.SetComplexityN(n);
}
BENCHMARK(BM_StackPushPop)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity();

// полный цикл FIFO: n enqueue + n dequeue -> O(N) суммарно, O(1) на операцию
static void BM_QueueEnqueueDequeue(benchmark::State& state) {
    const int n = state.range(0);
    for (auto _ : state) {
        Queue<int> q;
        for (int i = 0; i < n; ++i)
            q.enqueue(i);
        for (int i = 0; i < n; ++i) {
            int v = q.dequeue();
            benchmark::DoNotOptimize(v);
        }
    }
    state.SetItemsProcessed(state.iterations() * 2 * n);
    state.SetComplexityN(n);
}
BENCHMARK(BM_QueueEnqueueDequeue)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity();