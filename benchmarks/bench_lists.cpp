#include <benchmark/benchmark.h>

#include "doubly_linked_list.h"
#include "singly_linked_list.h"

// 1. pushBack списка: O(1) на операцию tail_ -> суммарно O(N)
static void BM_ListPushBack(benchmark::State &state) {
  const int n = state.range(0);
  for (auto _ : state) {
    SinglyLinkedList<int> list;
    for (int i = 0; i < n; ++i) {
      list.pushBack(i);
    }
    benchmark::DoNotOptimize(list);
  }
  state.SetItemsProcessed(state.iterations() * n);
  state.SetComplexityN(n);
}
BENCHMARK(BM_ListPushBack)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity();

// 2. вставка в начало: O(1) на операцию -> суммарно O(N)
static void BM_ListInsertAtBegin(benchmark::State &state) {
  const int n = state.range(0);
  for (auto _ : state) {
    SinglyLinkedList<int> list;
    for (int i = 0; i < n; ++i) {
      list.insertAt(0, i);
    }
    benchmark::DoNotOptimize(list);
  }
  state.SetComplexityN(n);
}
BENCHMARK(BM_ListInsertAtBegin)
    ->RangeMultiplier(2)
    ->Range(8, 4 << 10)
    ->Complexity();

// 3. доступ по индексу (берём середину): O(n) на операцию — обход указателей
static void BM_ListAt(benchmark::State &state) {
  const int n = state.range(0);
  SinglyLinkedList<int> list;
  for (int i = 0; i < n; ++i)
    list.pushBack(i);
  for (auto _ : state) {
    int value = list.at(static_cast<std::size_t>(n / 2));
    benchmark::DoNotOptimize(value);
  }
  state.SetItemsProcessed(state.iterations());
  state.SetComplexityN(n);
}
BENCHMARK(BM_ListAt)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity();

// 4ю removeBack: O(n) на операцию (ищем предпоследний) -> суммарно O(N^2)
static void BM_ListRemoveBack(benchmark::State &state) {
  const int n = state.range(0);
  for (auto _ : state) {
    state.PauseTiming();
    SinglyLinkedList<int> list;
    for (int i = 0; i < n; ++i)
      list.pushBack(i);
    state.ResumeTiming();
    for (int i = 0; i < n; ++i) {
      list.removeBack();
    }
    benchmark::DoNotOptimize(list);
  }
  state.SetComplexityN(n);
}
BENCHMARK(BM_ListRemoveBack)
    ->RangeMultiplier(2)
    ->Range(8, 4 << 10)
    ->Complexity();

// 5. поиск по значению (элемента нет): O(n) — полный обход списка
static void BM_ListContains(benchmark::State &state) {
  const int n = state.range(0);
  SinglyLinkedList<int> list;
  for (int i = 0; i < n; ++i)
    list.pushBack(i);
  for (auto _ : state) {
    bool found = list.contains(-1); // -1 нет в списке -> обходим всё
    benchmark::DoNotOptimize(found);
  }
  state.SetItemsProcessed(state.iterations());
  state.SetComplexityN(n);
}
BENCHMARK(BM_ListContains)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity();

// 6. removeBack двусвязного: O(1) на операцию -> суммарно O(N)
static void BM_DListRemoveBack(benchmark::State &state) {
  const int n = state.range(0);
  for (auto _ : state) {
    state.PauseTiming();
    DoublyLinkedList<int> list;
    for (int i = 0; i < n; ++i)
      list.pushBack(i);
    state.ResumeTiming();
    for (int i = 0; i < n; ++i) {
      list.removeBack();
    }
    benchmark::DoNotOptimize(list);
  }
  state.SetComplexityN(n);
}
BENCHMARK(BM_DListRemoveBack)
    ->RangeMultiplier(2)
    ->Range(8, 4 << 10)
    ->Complexity();

// 7. pushBack двусвязного: O(1), но узел тяжелее (два указателя вместо одного)
static void BM_DListPushBack(benchmark::State &state) {
  const int n = state.range(0);
  for (auto _ : state) {
    DoublyLinkedList<int> list;
    for (int i = 0; i < n; ++i)
      list.pushBack(i);
    benchmark::DoNotOptimize(list);
  }
  state.SetItemsProcessed(state.iterations() * n);
  state.SetComplexityN(n);
}
BENCHMARK(BM_DListPushBack)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10)
    ->Complexity();