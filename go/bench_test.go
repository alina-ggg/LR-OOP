package ads

import "testing"

const benchN = 4096

// массив: вставка в конец
func BenchmarkArrayPushBack(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		a := NewArray[int]()
		for v := 0; v < benchN; v++ {
			a.PushBack(v)
		}
	}
}

// односвязный: вставка в начало O(1) на операцию
func BenchmarkSinglyInsertAtBegin(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		l := NewSinglyLinkedList[int]()
		for v := 0; v < benchN; v++ {
			_ = l.InsertAt(0, v)
		}
	}
}

// односвязный: removeBack O(n) на операцию -> квадрат суммарно
func BenchmarkSinglyRemoveBack(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		l := NewSinglyLinkedList[int]()
		for v := 0; v < benchN; v++ {
			l.PushBack(v)
		}
		for v := 0; v < benchN; v++ {
			_ = l.RemoveBack()
		}
	}
}

// двусвязный: removeBack O(1) на операцию -> линейно суммарно
func BenchmarkDoublyRemoveBack(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		l := NewDoublyLinkedList[int]()
		for v := 0; v < benchN; v++ {
			l.PushBack(v)
		}
		for v := 0; v < benchN; v++ {
			_ = l.RemoveBack()
		}
	}
}

// хеш: вставка + поиск, average O(1)
func BenchmarkHashInsertGet(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		h := NewHashTable[int, int](HashInt)
		for v := 0; v < benchN; v++ {
			h.Insert(v, v)
		}
		for v := 0; v < benchN; v++ {
			_, _ = h.Get(v)
		}
	}
}

// дерево: вставка O(1)
func BenchmarkTreeInsert(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		tr := NewCompleteBinaryTree[int]()
		for v := 0; v < benchN; v++ {
			tr.Insert(v)
		}
	}
}
