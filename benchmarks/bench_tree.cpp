#include <benchmark/benchmark.h>

#include "complete_binary_tree.h"

// вставка = append в массив: амортизированная O(1), суммарно O(N)
static void BM_TreeInsert(benchmark::State &state) {
  const int n = state.range(0);
  for (auto _ : state) {
    CompleteBinaryTree<int> tree;
    for (int i = 0; i < n; ++i)
      tree.insert(i);
    benchmark::DoNotOptimize(tree);
  }
  state.SetItemsProcessed(state.iterations() * n);
  state.SetComplexityN(n);
}
BENCHMARK(BM_TreeInsert)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity();

// поиск отсутствующего элемента: полный перебор -> O(n) на операцию
static void BM_TreeContainsMiss(benchmark::State &state) {
  const int n = state.range(0);
  CompleteBinaryTree<int> tree;
  for (int i = 0; i < n; ++i)
    tree.insert(i);
  for (auto _ : state) {
    bool f = tree.contains(-1);
    benchmark::DoNotOptimize(f);
  }
  state.SetItemsProcessed(state.iterations());
  state.SetComplexityN(n);
}
BENCHMARK(BM_TreeContainsMiss)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10)
    ->Complexity();

// спуск от корня к листу по индексам детей: O(log n)
static void BM_TreeRootToLeaf(benchmark::State &state) {
  const int n = state.range(0);
  CompleteBinaryTree<int> tree;
  for (int i = 0; i < n; ++i)
    tree.insert(i);
  for (auto _ : state) {
    std::size_t idx = 0;
    while (tree.hasLeftChild(idx))
      idx = 2 * idx + 1;
    benchmark::DoNotOptimize(tree.at(idx));
  }
  state.SetComplexityN(n);
}
BENCHMARK(BM_TreeRootToLeaf)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10)
    ->Complexity();