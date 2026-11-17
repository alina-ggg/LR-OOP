package ads

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestTreeInsertAndRelations(t *testing.T) {
	tr := NewCompleteBinaryTree[int]()
	assert.True(t, tr.IsEmpty())
	assert.Equal(t, 0, tr.Height())
	_, err := tr.Root()
	assert.ErrorIs(t, err, ErrIndexOutOfRange)
	assert.ErrorIs(t, tr.RemoveLast(), ErrEmpty)

	for v := 1; v <= 7; v++ {
		tr.Insert(v)
	}
	assert.Equal(t, 7, tr.Size())
	assert.Equal(t, 3, tr.Height())

	root, err := tr.Root()
	require.NoError(t, err)
	assert.Equal(t, 1, root)
	lc, err := tr.LeftChildOf(0)
	require.NoError(t, err)
	assert.Equal(t, 2, lc)
	rc, err := tr.RightChildOf(0)
	require.NoError(t, err)
	assert.Equal(t, 3, rc)
	p, err := tr.ParentOf(3)
	require.NoError(t, err)
	assert.Equal(t, 2, p)

	assert.False(t, tr.HasParent(0))
	assert.False(t, tr.HasLeftChild(3))
	assert.False(t, tr.HasRightChild(3))
	assert.False(t, tr.HasLeftChild(10))
}

func TestTreeAccessorsErrors(t *testing.T) {
	tr := NewCompleteBinaryTree[int]()
	tr.Insert(1)
	tr.Insert(2)
	tr.Insert(3)

	_, err := tr.ParentOf(0)
	assert.ErrorIs(t, err, ErrIndexOutOfRange)
	_, err = tr.LeftChildOf(1)
	assert.ErrorIs(t, err, ErrIndexOutOfRange)
	_, err = tr.RightChildOf(1)
	assert.ErrorIs(t, err, ErrIndexOutOfRange)
	_, err = tr.At(3)
	assert.ErrorIs(t, err, ErrIndexOutOfRange)
	assert.ErrorIs(t, tr.Set(3, 0), ErrIndexOutOfRange)

	require.NoError(t, tr.Set(0, 10))
	root, err := tr.Root()
	require.NoError(t, err)
	assert.Equal(t, 10, root)
}

func TestTreeRemoveHeightContains(t *testing.T) {
	tr := NewCompleteBinaryTree[int]()
	for v := 1; v <= 7; v++ {
		tr.Insert(v)
	}
	require.NoError(t, tr.RemoveLast())
	assert.Equal(t, 6, tr.Size())
	assert.Equal(t, 3, tr.Height())
	assert.False(t, tr.HasRightChild(2))

	eq := func(a, b int) bool { return a == b }
	assert.True(t, tr.Contains(5, eq))
	assert.False(t, tr.Contains(9, eq))

	for !tr.IsEmpty() {
		require.NoError(t, tr.RemoveLast())
	}
	assert.Equal(t, 0, tr.Height())
	tr.Insert(1)
	assert.Equal(t, 1, tr.Height())
	tr.Clear()
	assert.True(t, tr.IsEmpty())
}
