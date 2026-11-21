package ads

type sllNode[T any] struct {
	value T
	next  *sllNode[T]
}

type SinglyLinkedList[T any] struct {
	head *sllNode[T]
	tail *sllNode[T]
	size int
}

func NewSinglyLinkedList[T any]() *SinglyLinkedList[T] { return &SinglyLinkedList[T]{} }

func (l *SinglyLinkedList[T]) Size() int     { return l.size }
func (l *SinglyLinkedList[T]) IsEmpty() bool { return l.size == 0 }

func (l *SinglyLinkedList[T]) PushFront(v T) {
	l.head = &sllNode[T]{value: v, next: l.head}
	if l.tail == nil {
		l.tail = l.head
	}
	l.size++
}

func (l *SinglyLinkedList[T]) PushBack(v T) {
	n := &sllNode[T]{value: v}
	if l.tail != nil {
		l.tail.next = n
	} else {
		l.head = n
	}
	l.tail = n
	l.size++
}

func (l *SinglyLinkedList[T]) InsertAt(index int, v T) error {
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
	prev := l.head
	for i := 0; i < index-1; i++ {
		prev = prev.next
	}
	prev.next = &sllNode[T]{value: v, next: prev.next}
	l.size++
	return nil
}

func (l *SinglyLinkedList[T]) RemoveFront() error {
	if l.size == 0 {
		return ErrEmpty
	}
	old := l.head
	l.head = old.next
	if l.head == nil {
		l.tail = nil
	}
	l.size--
	return nil
}

func (l *SinglyLinkedList[T]) RemoveBack() error {
	if l.size == 0 {
		return ErrEmpty
	}
	if l.size == 1 {
		return l.RemoveFront()
	}
	prev := l.head
	for prev.next != l.tail {
		prev = prev.next
	}
	prev.next = nil
	l.tail = prev
	l.size--
	return nil
}

func (l *SinglyLinkedList[T]) RemoveAt(index int) error {
	if index < 0 || index >= l.size {
		return ErrIndexOutOfRange
	}
	if index == 0 {
		return l.RemoveFront()
	}
	if index == l.size-1 {
		return l.RemoveBack()
	}
	prev := l.head
	for i := 0; i < index-1; i++ {
		prev = prev.next
	}
	prev.next = prev.next.next
	l.size--
	return nil
}

func (l *SinglyLinkedList[T]) At(index int) (T, error) {
	var zero T
	if index < 0 || index >= l.size {
		return zero, ErrIndexOutOfRange
	}
	cur := l.head
	for i := 0; i < index; i++ {
		cur = cur.next
	}
	return cur.value, nil
}

func (l *SinglyLinkedList[T]) Set(index int, v T) error {
	if index < 0 || index >= l.size {
		return ErrIndexOutOfRange
	}
	cur := l.head
	for i := 0; i < index; i++ {
		cur = cur.next
	}
	cur.value = v
	return nil
}

// Contains принимает предикат сравнения
func (l *SinglyLinkedList[T]) Contains(v T, eq func(a, b T) bool) bool {
	for cur := l.head; cur != nil; cur = cur.next {
		if eq(cur.value, v) {
			return true
		}
	}
	return false
}

func (l *SinglyLinkedList[T]) InsertBefore(v T, newValue T, eq func(a, b T) bool) bool {
	if l.head == nil {
		return false
	}
	// вставка перед головой
	if eq(l.head.value, v) {
		l.PushFront(newValue)
		return true
	}
	cur := l.head
	for cur.next != nil {
		if eq(cur.next.value, v) {
			n := &sllNode[T]{value: newValue, next: cur.next}
			cur.next = n
			l.size++
			return true
		}
		cur = cur.next
	}
	return false
}

func (l *SinglyLinkedList[T]) InsertAfter(v T, newValue T, eq func(a, b T) bool) bool {
	cur := l.head
	for cur != nil {
		if eq(cur.value, v) {
			n := &sllNode[T]{value: newValue, next: cur.next}
			cur.next = n
			if cur == l.tail {
				l.tail = n
			}
			l.size++
			return true
		}
		cur = cur.next
	}
	return false
}

func (l *SinglyLinkedList[T]) RemoveBefore(v T, eq func(a, b T) bool) bool {
	if l.head == nil || l.head.next == nil {
		return false
	}
	// если второй элемент == v, удаляем голову
	if eq(l.head.next.value, v) {
		err := l.RemoveFront()
		return err == nil
	}
	cur := l.head
	for cur.next != nil && cur.next.next != nil {
		if eq(cur.next.next.value, v) {
			cur.next = cur.next.next
			l.size--
			return true
		}
		cur = cur.next
	}
	return false
}

func (l *SinglyLinkedList[T]) RemoveAfter(v T, eq func(a, b T) bool) bool {
	cur := l.head
	for cur != nil {
		if eq(cur.value, v) {
			if cur.next == nil {
				return false // после v нет элементов
			}
			toDelete := cur.next
			cur.next = toDelete.next
			if toDelete == l.tail {
				l.tail = cur
			}
			l.size--
			return true
		}
		cur = cur.next
	}
	return false
}

func (l *SinglyLinkedList[T]) Clear() {
	l.head = nil
	l.tail = nil
	l.size = 0
}
