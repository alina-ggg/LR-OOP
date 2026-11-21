package ads

type dllNode[T any] struct {
	value T
	prev  *dllNode[T]
	next  *dllNode[T]
}

type DoublyLinkedList[T any] struct {
	head *dllNode[T]
	tail *dllNode[T]
	size int
}

func NewDoublyLinkedList[T any]() *DoublyLinkedList[T] { return &DoublyLinkedList[T]{} }

func (l *DoublyLinkedList[T]) Size() int     { return l.size }
func (l *DoublyLinkedList[T]) IsEmpty() bool { return l.size == 0 }

func (l *DoublyLinkedList[T]) PushFront(v T) {
	n := &dllNode[T]{value: v, next: l.head}
	if l.head != nil {
		l.head.prev = n
	} else {
		l.tail = n
	}
	l.head = n
	l.size++
}

func (l *DoublyLinkedList[T]) PushBack(v T) {
	n := &dllNode[T]{value: v, prev: l.tail}
	if l.tail != nil {
		l.tail.next = n
	} else {
		l.head = n
	}
	l.tail = n
	l.size++
}

// nodeAt идёт с ближайшего конца: O(n/2) вместо O(n).
func (l *DoublyLinkedList[T]) nodeAt(index int) *dllNode[T] {
	if index < l.size/2 {
		cur := l.head
		for i := 0; i < index; i++ {
			cur = cur.next
		}
		return cur
	}
	cur := l.tail
	for i := l.size - 1; i > index; i-- {
		cur = cur.prev
	}
	return cur
}

func (l *DoublyLinkedList[T]) InsertAt(index int, v T) error {
	if index < 0 || index > l.size {
		return ErrIndexOutOfRange
	}
	if index == 0 {
		l.PushFront(v)
		return nil
	}
	if index == l.size {
		l.PushBack(v)
		return nil
	}
	cur := l.nodeAt(index)
	n := &dllNode[T]{value: v, prev: cur.prev, next: cur}
	cur.prev.next = n
	cur.prev = n
	l.size++
	return nil
}

func (l *DoublyLinkedList[T]) RemoveFront() error {
	if l.size == 0 {
		return ErrEmpty
	}
	old := l.head
	l.head = old.next
	if l.head != nil {
		l.head.prev = nil
	} else {
		l.tail = nil
	}
	l.size--
	return nil
}

// RemoveBack — O(1): предпоследний узел известен через prev.
func (l *DoublyLinkedList[T]) RemoveBack() error {
	if l.size == 0 {
		return ErrEmpty
	}
	old := l.tail
	l.tail = old.prev
	if l.tail != nil {
		l.tail.next = nil
	} else {
		l.head = nil
	}
	l.size--
	return nil
}

func (l *DoublyLinkedList[T]) RemoveAt(index int) error {
	if index < 0 || index >= l.size {
		return ErrIndexOutOfRange
	}
	if index == 0 {
		return l.RemoveFront()
	}
	if index == l.size-1 {
		return l.RemoveBack()
	}
	cur := l.nodeAt(index)
	cur.prev.next = cur.next
	cur.next.prev = cur.prev
	l.size--
	return nil
}

func (l *DoublyLinkedList[T]) At(index int) (T, error) {
	var zero T
	if index < 0 || index >= l.size {
		return zero, ErrIndexOutOfRange
	}
	return l.nodeAt(index).value, nil
}

func (l *DoublyLinkedList[T]) Set(index int, v T) error {
	if index < 0 || index >= l.size {
		return ErrIndexOutOfRange
	}
	l.nodeAt(index).value = v
	return nil
}

func (l *DoublyLinkedList[T]) Contains(v T, eq func(a, b T) bool) bool {
	for cur := l.head; cur != nil; cur = cur.next {
		if eq(cur.value, v) {
			return true
		}
	}
	return false
}

func (l *DoublyLinkedList[T]) Clear() {
	l.head = nil
	l.tail = nil
	l.size = 0
}

func (l *DoublyLinkedList[T]) InsertBefore(v T, newValue T, eq func(a, b T) bool) bool {
	for cur := l.head; cur != nil; cur = cur.next {
		if eq(cur.value, v) {
			n := &dllNode[T]{value: newValue, prev: cur.prev, next: cur}
			if cur.prev != nil {
				cur.prev.next = n
			} else {
				l.head = n
			}
			cur.prev = n
			l.size++
			return true
		}
	}
	return false
}

func (l *DoublyLinkedList[T]) InsertAfter(v T, newValue T, eq func(a, b T) bool) bool {
	for cur := l.head; cur != nil; cur = cur.next {
		if eq(cur.value, v) {
			n := &dllNode[T]{value: newValue, prev: cur, next: cur.next}
			if cur.next != nil {
				cur.next.prev = n
			} else {
				l.tail = n
			}
			cur.next = n
			l.size++
			return true
		}
	}
	return false
}

func (l *DoublyLinkedList[T]) RemoveBefore(v T, eq func(a, b T) bool) bool {
	for cur := l.head; cur != nil; cur = cur.next {
		if eq(cur.value, v) {
			if cur.prev == nil {
				return false // перед v нет элементов
			}
			toDelete := cur.prev
			if toDelete.prev != nil {
				toDelete.prev.next = cur
			} else {
				l.head = cur
			}
			cur.prev = toDelete.prev
			l.size--
			return true
		}
	}
	return false
}

func (l *DoublyLinkedList[T]) RemoveAfter(v T, eq func(a, b T) bool) bool {
	for cur := l.head; cur != nil; cur = cur.next {
		if eq(cur.value, v) {
			if cur.next == nil {
				return false // после v нет элементов
			}
			toDelete := cur.next
			if toDelete.next != nil {
				toDelete.next.prev = cur
			} else {
				l.tail = cur
			}
			cur.next = toDelete.next
			l.size--
			return true
		}
	}
	return false
}
