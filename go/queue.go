package ads

// Queue — FIFO-адаптер над SinglyLinkedList.
type Queue[T any] struct {
	data SinglyLinkedList[T]
}

func NewQueue[T any]() *Queue[T] { return &Queue[T]{} }

func (q *Queue[T]) Enqueue(v T) { q.data.PushBack(v) }

func (q *Queue[T]) Dequeue() (T, error) {
	var zero T
	if q.data.IsEmpty() {
		return zero, ErrEmpty
	}
	v, err := q.data.At(0)
	if err != nil {
		return zero, err
	}
	if err := q.data.RemoveFront(); err != nil {
		return zero, err
	}
	return v, nil
}

func (q *Queue[T]) Front() (T, error) {
	var zero T
	if q.data.IsEmpty() {
		return zero, ErrEmpty
	}
	return q.data.At(0)
}

func (q *Queue[T]) Size() int     { return q.data.Size() }
func (q *Queue[T]) IsEmpty() bool { return q.data.IsEmpty() }
func (q *Queue[T]) Clear()        { q.data.Clear() }
