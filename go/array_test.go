package ads

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestArrayPushPopGrowth(t *testing.T) {
	a := NewArray[int]()
	assert.True(t, a.IsEmpty())
	assert.Equal(t, 0, a.Size())
	assert.Equal(t, 0, a.Capacity())

	for i := 1; i <= 5; i++ {
		a.PushBack(i * 10)
	}
	assert.Equal(t, 5, a.Size())
	assert.Equal(t, 8, a.Capacity()) // 1->2->4->8

	v, err := a.PopBack()
	require.NoError(t, err)
	assert.Equal(t, 50, v)
	assert.Equal(t, 4, a.Size())
}

func TestArrayInsertRemoveAt(t *testing.T) {
	a := NewArray[int]()
	require.NoError(t, a.InsertAt(0, 2))
	require.NoError(t, a.InsertAt(1, 4))
	require.NoError(t, a.InsertAt(0, 1))
	require.NoError(t, a.InsertAt(2, 3))
	require.NoError(t, a.InsertAt(4, 5))
	for i := 0; i < 5; i++ {
		got, err := a.At(i)
		require.NoError(t, err)
		assert.Equal(t, i+1, got)
	}
	assert.ErrorIs(t, a.InsertAt(9, 0), ErrIndexOutOfRange)

	require.NoError(t, a.RemoveAt(2))
	got, err := a.At(2)
	require.NoError(t, err)
	assert.Equal(t, 4, got)
	assert.ErrorIs(t, a.RemoveAt(-1), ErrIndexOutOfRange)
	assert.ErrorIs(t, a.RemoveAt(4), ErrIndexOutOfRange)
}

func TestArrayAccessErrors(t *testing.T) {
	a := NewArray[int]()
	_, err := a.PopBack()
	assert.ErrorIs(t, err, ErrEmpty)
	_, err = a.At(0)
	assert.ErrorIs(t, err, ErrIndexOutOfRange)
	assert.ErrorIs(t, a.Set(0, 1), ErrIndexOutOfRange)

	a.PushBack(7)
	require.NoError(t, a.Set(0, 9))
	got, err := a.At(0)
	require.NoError(t, err)
	assert.Equal(t, 9, got)

	a.Clear()
	assert.True(t, a.IsEmpty())
}

func TestArrayCapacityConstructor(t *testing.T) {
	a := NewArrayCapacity[int](4)
	assert.Equal(t, 4, a.Capacity())
	assert.Equal(t, 0, a.Size())
	a.PushBack(1)
	assert.Equal(t, 4, a.Capacity()) // роста ещё не было
}

func TestArrayStrings(t *testing.T) {
	a := NewArray[string]()
	a.PushBack("hello")
	a.PushBack("world")
	got, err := a.At(1)
	require.NoError(t, err)
	assert.Equal(t, "world", got)
}

func TestArrayNegativeCapacity(t *testing.T) {
	a := NewArrayCapacity[int](-5)
	assert.Equal(t, 0, a.Capacity())
	assert.Equal(t, 0, a.Size())
}

func TestArrayContains(t *testing.T) {
	a := NewArray[int]()
	a.PushBack(10)
	a.PushBack(20)
	a.PushBack(30)

	// Функция сравнения для int
	eqInt := func(a, b int) bool { return a == b }

	assert.True(t, a.Contains(10, eqInt))
	assert.True(t, a.Contains(20, eqInt))
	assert.True(t, a.Contains(30, eqInt))
	assert.False(t, a.Contains(0, eqInt))
	assert.False(t, a.Contains(99, eqInt))

	// Пустой массив
	empty := NewArray[int]()
	assert.False(t, empty.Contains(1, eqInt))
}
