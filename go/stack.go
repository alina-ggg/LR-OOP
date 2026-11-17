package ads

// Stack — LIFO-адаптер над Array
type Stack[T any] struct {
	data Array[T]
}

func NewStack[T any]() *Stack[T] { return &Stack[T]{} }

func (s *Stack[T]) Push(v T)        { s.data.PushBack(v) }
func (s *Stack[T]) Pop() (T, error) { return s.data.PopBack() }

func (s *Stack[T]) Top() (T, error) {
	var zero T
	if s.data.IsEmpty() {
		return zero, ErrEmpty
	}
	return s.data.At(s.data.Size() - 1)
}

func (s *Stack[T]) Size() int     { return s.data.Size() }
func (s *Stack[T]) IsEmpty() bool { return s.data.IsEmpty() }
func (s *Stack[T]) Clear()        { s.data.Clear() }
