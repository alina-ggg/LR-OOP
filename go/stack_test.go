package ads

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestStackLIFO(t *testing.T) {
	s := NewStack[int]()
	assert.True(t, s.IsEmpty())
	_, err := s.Pop()
	assert.ErrorIs(t, err, ErrEmpty)
	_, err = s.Top()
	assert.ErrorIs(t, err, ErrEmpty)

	s.Push(1)
	s.Push(2)
	s.Push(3)
	top, err := s.Top()
	require.NoError(t, err)
	assert.Equal(t, 3, top)

	v, err := s.Pop()
	require.NoError(t, err)
	assert.Equal(t, 3, v)
	assert.Equal(t, 2, s.Size())

	s.Clear()
	assert.True(t, s.IsEmpty())
}
