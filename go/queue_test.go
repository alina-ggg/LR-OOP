package ads

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestQueueFIFO(t *testing.T) {
	q := NewQueue[int]()
	assert.True(t, q.IsEmpty())
	_, err := q.Dequeue()
	assert.ErrorIs(t, err, ErrEmpty)
	_, err = q.Front()
	assert.ErrorIs(t, err, ErrEmpty)

	q.Enqueue(1)
	q.Enqueue(2)
	q.Enqueue(3)
	f, err := q.Front()
	require.NoError(t, err)
	assert.Equal(t, 1, f)

	v, err := q.Dequeue()
	require.NoError(t, err)
	assert.Equal(t, 1, v)
	assert.Equal(t, 2, q.Size())

	q.Clear()
	assert.True(t, q.IsEmpty())
}
