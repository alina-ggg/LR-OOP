package ads

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestDoublyPushAndAt(t *testing.T) {
	l := NewDoublyLinkedList[int]()
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

func TestDoublyNodeAtBothDirections(t *testing.T) {
	l := NewDoublyLinkedList[int]()
	for v := 0; v < 6; v++ {
		l.PushBack(v)
	}
	got, err := l.At(1) // обход с головы
	require.NoError(t, err)
	assert.Equal(t, 1, got)
	got, err = l.At(4) // обход с хвоста
	require.NoError(t, err)
	assert.Equal(t, 4, got)
	got, err = l.At(5) // сам хвост
	require.NoError(t, err)
	assert.Equal(t, 5, got)
}

func TestDoublyInsertRemove(t *testing.T) {
	l := NewDoublyLinkedList[int]()
	require.NoError(t, l.InsertAt(0, 2))
	require.NoError(t, l.InsertAt(1, 4))
	require.NoError(t, l.InsertAt(0, 1))
	require.NoError(t, l.InsertAt(2, 3))
	require.NoError(t, l.InsertAt(4, 5))
	assert.Equal(t, 5, l.Size())

	require.NoError(t, l.RemoveAt(1)) // середина, 1-я половина
	require.NoError(t, l.RemoveAt(2)) // середина, 2-я половина
	require.NoError(t, l.RemoveAt(0)) // голова
	require.NoError(t, l.RemoveAt(1)) // хвост
	assert.Equal(t, 1, l.Size())

	require.NoError(t, l.RemoveBack()) // единственный узел
	assert.True(t, l.IsEmpty())
	assert.ErrorIs(t, l.RemoveFront(), ErrEmpty)
	assert.ErrorIs(t, l.RemoveBack(), ErrEmpty)
	assert.ErrorIs(t, l.InsertAt(2, 0), ErrIndexOutOfRange)
	assert.ErrorIs(t, l.RemoveAt(-1), ErrIndexOutOfRange)
}

func TestDoublyRemoveBackAndContains(t *testing.T) {
	l := NewDoublyLinkedList[int]()
	l.PushBack(1)
	l.PushBack(2)
	l.PushBack(3)
	require.NoError(t, l.RemoveBack()) // несколько узлов
	assert.Equal(t, 2, l.Size())
	got, err := l.At(1)
	require.NoError(t, err)
	assert.Equal(t, 2, got)

	eq := func(a, b int) bool { return a == b }
	assert.True(t, l.Contains(1, eq))
	assert.False(t, l.Contains(9, eq))
	l.Clear()
	assert.True(t, l.IsEmpty())
	assert.False(t, l.Contains(1, eq))
}

func TestDoublyRemoveFrontSingle(t *testing.T) {
	l := NewDoublyLinkedList[int]()
	l.PushBack(1)
	require.NoError(t, l.RemoveFront())
	assert.True(t, l.IsEmpty())
}
