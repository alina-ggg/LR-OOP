package ads

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

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
