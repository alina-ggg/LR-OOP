package ads

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func eqInt(a, b int) bool { return a == b }

func TestSinglyContains(t *testing.T) {
	l := NewSinglyLinkedList[int]()
	l.PushBack(10)
	l.PushBack(20)
	l.PushBack(30)

	assert.True(t, l.Contains(10, eqInt))
	assert.True(t, l.Contains(20, eqInt))
	assert.True(t, l.Contains(30, eqInt))
	assert.False(t, l.Contains(0, eqInt))
	assert.False(t, l.Contains(99, eqInt))

	// Пустой список
	empty := NewSinglyLinkedList[int]()
	assert.False(t, empty.Contains(1, eqInt))
}

func TestSinglyInsertBefore(t *testing.T) {
	l := NewSinglyLinkedList[int]()
	l.PushBack(1)
	l.PushBack(2)
	l.PushBack(3)

	// Вставка перед существующим элементом
	assert.True(t, l.InsertBefore(2, 99, eqInt)) // 1 99 2 3
	assert.Equal(t, 4, l.Size())
	v, err := l.At(1)
	require.NoError(t, err)
	assert.Equal(t, 99, v)

	// Вставка перед головой
	assert.True(t, l.InsertBefore(1, 0, eqInt)) // 0 1 99 2 3
	assert.Equal(t, 5, l.Size())
	v, err = l.At(0)
	require.NoError(t, err)
	assert.Equal(t, 0, v)

	// Вставка перед несуществующим элементом
	assert.False(t, l.InsertBefore(999, 50, eqInt))
	assert.Equal(t, 5, l.Size())

	// Пустой список
	empty := NewSinglyLinkedList[int]()
	assert.False(t, empty.InsertBefore(1, 99, eqInt))
}

func TestSinglyInsertAfter(t *testing.T) {
	l := NewSinglyLinkedList[int]()
	l.PushBack(1)
	l.PushBack(2)
	l.PushBack(3)

	// Вставка после существующего элемента
	assert.True(t, l.InsertAfter(2, 99, eqInt)) // 1 2 99 3
	assert.Equal(t, 4, l.Size())
	v, err := l.At(2)
	require.NoError(t, err)
	assert.Equal(t, 99, v)

	// Вставка после хвоста
	assert.True(t, l.InsertAfter(3, 4, eqInt)) // 1 2 99 3 4
	assert.Equal(t, 5, l.Size())
	v, err = l.At(4)
	require.NoError(t, err)
	assert.Equal(t, 4, v)

	// Вставка после несуществующего
	assert.False(t, l.InsertAfter(999, 50, eqInt))

	// Пустой список
	empty := NewSinglyLinkedList[int]()
	assert.False(t, empty.InsertAfter(1, 99, eqInt))
}

func TestSinglyRemoveBefore(t *testing.T) {
	l := NewSinglyLinkedList[int]()
	l.PushBack(1)
	l.PushBack(2)
	l.PushBack(3)
	l.PushBack(4)

	// Удаление перед существующим
	assert.True(t, l.RemoveBefore(3, eqInt)) // 1 3 4
	assert.Equal(t, 3, l.Size())
	v, err := l.At(1)
	require.NoError(t, err)
	assert.Equal(t, 3, v)

	// Удаление перед головой — невозможно
	assert.False(t, l.RemoveBefore(1, eqInt))
	assert.Equal(t, 3, l.Size())

	// Удаление перед несуществующим
	assert.False(t, l.RemoveBefore(999, eqInt))

	// Пустой список
	empty := NewSinglyLinkedList[int]()
	assert.False(t, empty.RemoveBefore(1, eqInt))
}

func TestSinglyRemoveAfter(t *testing.T) {
	l := NewSinglyLinkedList[int]()
	l.PushBack(1)
	l.PushBack(2)
	l.PushBack(3)
	l.PushBack(4)

	// Удаление после существующего (удалить 2, т.к. после 1)
	assert.True(t, l.RemoveAfter(1, eqInt)) // 1 3 4
	assert.Equal(t, 3, l.Size())
	v, err := l.At(1)
	require.NoError(t, err)
	assert.Equal(t, 3, v)

	// Удаление после хвоста — невозможно
	assert.False(t, l.RemoveAfter(4, eqInt))
	assert.Equal(t, 3, l.Size())

	// Удаление после несуществующего
	assert.False(t, l.RemoveAfter(999, eqInt))

	// Пустой список
	empty := NewSinglyLinkedList[int]()
	assert.False(t, empty.RemoveAfter(1, eqInt))
}
func TestSinglyPushAndAt(t *testing.T) {
	l := NewSinglyLinkedList[int]()
	assert.True(t, l.IsEmpty())
	l.PushFront(2)
	l.PushFront(1)
	l.PushBack(3)
	assert.Equal(t, 3, l.Size())
	for i, want := range []int{1, 2, 3} {
		got, err := l.At(i)
		require.NoError(t, err)
		assert.Equal(t, want, got)
	}
	require.NoError(t, l.Set(1, 20))
	got, err := l.At(1)
	require.NoError(t, err)
	assert.Equal(t, 20, got)
	_, err = l.At(3)
	assert.ErrorIs(t, err, ErrIndexOutOfRange)
	assert.ErrorIs(t, l.Set(-1, 0), ErrIndexOutOfRange)
}

func TestSinglyInsertRemove(t *testing.T) {
	l := NewSinglyLinkedList[int]()
	require.NoError(t, l.InsertAt(0, 2))
	require.NoError(t, l.InsertAt(1, 4))
	require.NoError(t, l.InsertAt(0, 1))
	require.NoError(t, l.InsertAt(2, 3))
	require.NoError(t, l.InsertAt(4, 5))
	assert.Equal(t, 5, l.Size())

	require.NoError(t, l.RemoveAt(2)) // середина
	require.NoError(t, l.RemoveAt(0)) // голова
	require.NoError(t, l.RemoveAt(2)) // хвост
	assert.Equal(t, 2, l.Size())
	got, err := l.At(0)
	require.NoError(t, err)
	assert.Equal(t, 2, got)
	got, err = l.At(1)
	require.NoError(t, err)
	assert.Equal(t, 4, got)

	assert.ErrorIs(t, l.InsertAt(9, 0), ErrIndexOutOfRange)
	assert.ErrorIs(t, l.RemoveAt(-1), ErrIndexOutOfRange)
}

func TestSinglyRemoveBackAndContains(t *testing.T) {
	l := NewSinglyLinkedList[int]()
	assert.ErrorIs(t, l.RemoveBack(), ErrEmpty)
	assert.ErrorIs(t, l.RemoveFront(), ErrEmpty)

	l.PushBack(1)
	require.NoError(t, l.RemoveBack()) // единственный
	assert.True(t, l.IsEmpty())

	l.PushBack(1)
	l.PushBack(2)
	l.PushBack(3)
	require.NoError(t, l.RemoveBack()) // несколько
	assert.Equal(t, 2, l.Size())

	eq := func(a, b int) bool { return a == b }
	assert.True(t, l.Contains(1, eq))
	assert.True(t, l.Contains(2, eq))
	assert.False(t, l.Contains(9, eq))

	l.Clear()
	assert.True(t, l.IsEmpty())
	assert.False(t, l.Contains(1, eq))
}
