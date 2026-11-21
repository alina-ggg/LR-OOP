package ads

// узел i -> дети 2i+1, 2i+2, родитель (i-1)/2.
type CompleteBinaryTree[T any] struct {
	nodes []T
}

func NewCompleteBinaryTree[T any]() *CompleteBinaryTree[T] { return &CompleteBinaryTree[T]{} }

func (t *CompleteBinaryTree[T]) Size() int     { return len(t.nodes) }
func (t *CompleteBinaryTree[T]) IsEmpty() bool { return len(t.nodes) == 0 }

func (t *CompleteBinaryTree[T]) Insert(v T) { t.nodes = append(t.nodes, v) }

func (t *CompleteBinaryTree[T]) RemoveLast() error {
	if len(t.nodes) == 0 {
		return ErrEmpty
	}
	t.nodes = t.nodes[:len(t.nodes)-1]
	return nil
}

func (t *CompleteBinaryTree[T]) Root() (T, error) { return t.At(0) }

func (t *CompleteBinaryTree[T]) At(index int) (T, error) {
	var zero T
	if index < 0 || index >= len(t.nodes) {
		return zero, ErrIndexOutOfRange
	}
	return t.nodes[index], nil
}

func (t *CompleteBinaryTree[T]) Set(index int, v T) error {
	if index < 0 || index >= len(t.nodes) {
		return ErrIndexOutOfRange
	}
	t.nodes[index] = v
	return nil
}

func (t *CompleteBinaryTree[T]) HasParent(index int) bool {
	return index > 0 && index < len(t.nodes)
}

func (t *CompleteBinaryTree[T]) HasLeftChild(index int) bool { return 2*index+1 < len(t.nodes) }

func (t *CompleteBinaryTree[T]) HasRightChild(index int) bool { return 2*index+2 < len(t.nodes) }

func (t *CompleteBinaryTree[T]) ParentOf(index int) (T, error) {
	var zero T
	if !t.HasParent(index) {
		return zero, ErrIndexOutOfRange
	}
	return t.nodes[(index-1)/2], nil
}

func (t *CompleteBinaryTree[T]) LeftChildOf(index int) (T, error) {
	var zero T
	if !t.HasLeftChild(index) {
		return zero, ErrIndexOutOfRange
	}
	return t.nodes[2*index+1], nil
}

func (t *CompleteBinaryTree[T]) RightChildOf(index int) (T, error) {
	var zero T
	if !t.HasRightChild(index) {
		return zero, ErrIndexOutOfRange
	}
	return t.nodes[2*index+2], nil
}

func (t *CompleteBinaryTree[T]) Contains(v T, eq func(a, b T) bool) bool {
	for _, n := range t.nodes {
		if eq(n, v) {
			return true
		}
	}
	return false
}

// Height = floor(log2(n)) + 1, считаем сдвигами.
func (t *CompleteBinaryTree[T]) Height() int {
	h := 0
	for n := len(t.nodes); n > 0; n >>= 1 {
		h++
	}
	return h
}

func (t *CompleteBinaryTree[T]) Clear() { t.nodes = nil }
func (t *CompleteBinaryTree[T]) IsComplete() bool {
	n := len(t.nodes)
	if n == 0 {
		return true
	}
	for i := 0; i < n; i++ {
		left := 2*i + 1
		right := 2*i + 2
		if left < n && right >= n {
			for j := i + 1; j < n; j++ {
				if 2*j+1 < n {
					return false
				}
			}
			return true
		}
		if left >= n && right < n {
			return false
		}
	}
	return true
}
