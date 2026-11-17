package ads

type Array[T any] struct {
	data     []T
	size     int
	capacity int
}

func NewArray[T any]() *Array[T] { return &Array[T]{} }

func NewArrayCapacity[T any](capacity int) *Array[T] {
	if capacity < 0 {
		capacity = 0
	}
	return &Array[T]{data: make([]T, capacity), capacity: capacity}
}

func (a *Array[T]) Size() int     { return a.size }
func (a *Array[T]) Capacity() int { return a.capacity }
func (a *Array[T]) IsEmpty() bool { return a.size == 0 }

func (a *Array[T]) grow(newCap int) {
	newData := make([]T, newCap)
	copy(newData, a.data[:a.size])
	a.data = newData
	a.capacity = newCap
}

func (a *Array[T]) PushBack(v T) {
	if a.size == a.capacity {
		newCap := 1
		if a.capacity > 0 {
			newCap = a.capacity * 2
		}
		a.grow(newCap)
	}
	a.data[a.size] = v
	a.size++
}

func (a *Array[T]) PopBack() (T, error) {
	var zero T
	if a.size == 0 {
		return zero, ErrEmpty
	}
	a.size--
	return a.data[a.size], nil
}

func (a *Array[T]) InsertAt(index int, v T) error {
	if index < 0 || index > a.size {
		return ErrIndexOutOfRange
	}
	if a.size == a.capacity {
		newCap := 1
		if a.capacity > 0 {
			newCap = a.capacity * 2
		}
		a.grow(newCap)
	}
	copy(a.data[index+1:a.size+1], a.data[index:a.size])
	a.data[index] = v
	a.size++
	return nil
}

func (a *Array[T]) RemoveAt(index int) error {
	if index < 0 || index >= a.size {
		return ErrIndexOutOfRange
	}
	copy(a.data[index:a.size-1], a.data[index+1:a.size])
	a.size--
	return nil
}

func (a *Array[T]) At(index int) (T, error) {
	var zero T
	if index < 0 || index >= a.size {
		return zero, ErrIndexOutOfRange
	}
	return a.data[index], nil
}

func (a *Array[T]) Set(index int, v T) error {
	if index < 0 || index >= a.size {
		return ErrIndexOutOfRange
	}
	a.data[index] = v
	return nil
}

func (a *Array[T]) Clear() { a.size = 0 }
