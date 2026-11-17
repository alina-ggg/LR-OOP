package ads

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestHashInsertGet(t *testing.T) {
	h := NewHashTable[int, string](HashInt)
	assert.True(t, h.IsEmpty())
	_, err := h.Get(1)
	assert.ErrorIs(t, err, ErrKeyNotFound)

	h.Insert(1, "one")
	h.Insert(2, "two")
	v, err := h.Get(2)
	require.NoError(t, err)
	assert.Equal(t, "two", v)

	h.Insert(2, "TWO") // обновление: размер не растёт
	v, err = h.Get(2)
	require.NoError(t, err)
	assert.Equal(t, "TWO", v)
	assert.Equal(t, 2, h.Size())
}

func TestHashStringKeys(t *testing.T) {
	h := NewHashTable[string, int](HashString)
	h.Insert("alice", 20)
	h.Insert("bob", 25)
	assert.True(t, h.Contains("alice"))
	v, err := h.Get("bob")
	require.NoError(t, err)
	assert.Equal(t, 25, v)

	require.NoError(t, h.Remove("bob"))
	assert.False(t, h.Contains("bob"))
	assert.ErrorIs(t, h.Remove("bob"), ErrKeyNotFound)
}

// коллизия: три ключа в одной цепочке.
func TestHashCollisionChaining(t *testing.T) {
	h := NewHashTable[int, int](HashInt)
	a := 0
	b, c := -1, -1
	for k := 1; k < 10000 && (b < 0 || c < 0); k++ {
		if h.BucketIndexOf(k) == h.BucketIndexOf(a) {
			if b < 0 {
				b = k
			} else if c < 0 {
				c = k
			}
		}
	}
	require.NotEqual(t, -1, b)
	require.NotEqual(t, -1, c)

	h.Insert(a, 10)
	h.Insert(b, 20)
	h.Insert(c, 30)
	idx := h.BucketIndexOf(a)
	assert.Equal(t, 3, h.BucketSize(idx))

	va, err := h.Get(a)
	require.NoError(t, err)
	assert.Equal(t, 10, va)
	vc, err := h.Get(c)
	require.NoError(t, err)
	assert.Equal(t, 30, vc)

	require.NoError(t, h.Remove(b)) // середина цепочки
	assert.Equal(t, 2, h.BucketSize(idx))
	require.NoError(t, h.Remove(a)) // голова
	require.NoError(t, h.Remove(c)) // хвост
	assert.True(t, h.IsEmpty())
}

func TestHashRehash(t *testing.T) {
	h := NewHashTable[int, int](HashInt)
	for i := 0; i < 100; i++ {
		h.Insert(i, i*2)
	}
	assert.Equal(t, 100, h.Size())
	assert.Greater(t, h.BucketCount(), defaultBuckets)
	assert.LessOrEqual(t, h.LoadFactor(), maxLoadFactor)
	for i := 0; i < 100; i++ {
		v, err := h.Get(i)
		require.NoError(t, err)
		assert.Equal(t, i*2, v)
	}
	h.Clear()
	assert.True(t, h.IsEmpty())
	assert.Equal(t, defaultBuckets, h.BucketCount())
}
