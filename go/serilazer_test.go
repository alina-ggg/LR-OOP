package ads

import (
	"bytes"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestTextArrayRoundTrip(t *testing.T) {
	src := NewArray[int]()
	for i := 1; i <= 3; i++ {
		src.PushBack(i * 10)
	}
	var buf bytes.Buffer
	require.NoError(t, SaveTextArray(&buf, src))
	assert.Contains(t, buf.String(), "ARRAY 3")

	dst := NewArray[int]()
	require.NoError(t, LoadTextArray(&buf, dst))
	assert.Equal(t, 3, dst.Size())
	for i := 0; i < 3; i++ {
		v, err := dst.At(i)
		require.NoError(t, err)
		assert.Equal(t, (i+1)*10, v)
	}
}

func TestTextStringArrayRoundTrip(t *testing.T) {
	src := NewArray[string]()
	src.PushBack("hello world")
	src.PushBack("")
	src.PushBack("line\nbreak")
	var buf bytes.Buffer
	require.NoError(t, SaveTextArray(&buf, src))

	dst := NewArray[string]()
	require.NoError(t, LoadTextArray(&buf, dst))
	assert.Equal(t, 3, dst.Size())
	v, err := dst.At(2)
	require.NoError(t, err)
	assert.Equal(t, "line\nbreak", v) // Quote экранирует перевод строки
}

func TestTextListsRoundTrip(t *testing.T) {
	s := NewSinglyLinkedList[int]()
	d := NewDoublyLinkedList[int]()
	for i := 0; i < 3; i++ {
		s.PushBack(i)
		d.PushBack(i)
	}
	var sb, db bytes.Buffer
	require.NoError(t, SaveTextSingly(&sb, s))
	require.NoError(t, SaveTextDoubly(&db, d))

	s2 := NewSinglyLinkedList[int]()
	d2 := NewDoublyLinkedList[int]()
	require.NoError(t, LoadTextSingly(&sb, s2))
	require.NoError(t, LoadTextDoubly(&db, d2))
	assert.Equal(t, 3, s2.Size())
	assert.Equal(t, 3, d2.Size())
	v, err := s2.At(1)
	require.NoError(t, err)
	assert.Equal(t, 1, v)
	v, err = d2.At(2)
	require.NoError(t, err)
	assert.Equal(t, 2, v)
}

func TestTextStackQueueRoundTrip(t *testing.T) {
	st := NewStack[int]()
	q := NewQueue[int]()
	for i := 1; i <= 3; i++ {
		st.Push(i)
		q.Enqueue(i)
	}
	var stb, qb bytes.Buffer
	require.NoError(t, SaveTextStack(&stb, st))
	require.NoError(t, SaveTextQueue(&qb, q))
	assert.Equal(t, 3, st.Size()) // save не разрушает оригинал
	assert.Equal(t, 3, q.Size())

	st2 := NewStack[int]()
	q2 := NewQueue[int]()
	require.NoError(t, LoadTextStack(&stb, st2))
	require.NoError(t, LoadTextQueue(&qb, q2))
	v, err := st2.Pop()
	require.NoError(t, err)
	assert.Equal(t, 3, v) // LIFO сохранён
	v, err = q2.Dequeue()
	require.NoError(t, err)
	assert.Equal(t, 1, v) // FIFO сохранён
}

func TestTextHashTreeRoundTrip(t *testing.T) {
	h := NewHashTable[string, int](HashString)
	h.Insert("alice", 20)
	h.Insert("bob", 25)
	var hb bytes.Buffer
	require.NoError(t, SaveTextHashTable(&hb, h))

	h2 := NewHashTable[string, int](HashString)
	require.NoError(t, LoadTextHashTable(&hb, h2))
	assert.Equal(t, 2, h2.Size())
	v, err := h2.Get("alice")
	require.NoError(t, err)
	assert.Equal(t, 20, v)

	tr := NewCompleteBinaryTree[int]()
	for i := 1; i <= 7; i++ {
		tr.Insert(i)
	}
	var tb bytes.Buffer
	require.NoError(t, SaveTextTree(&tb, tr))

	tr2 := NewCompleteBinaryTree[int]()
	require.NoError(t, LoadTextTree(&tb, tr2))
	assert.Equal(t, 7, tr2.Size())
	assert.Equal(t, 3, tr2.Height())
	r, err := tr2.Root()
	require.NoError(t, err)
	assert.Equal(t, 1, r)
}

func TestTextBadFormat(t *testing.T) {
	garbage := bytes.NewBufferString("NOT_A_TAG 3\n1\n2\n3\n")
	dst := NewArray[int]()
	assert.ErrorIs(t, LoadTextArray(garbage, dst), ErrBadFormat)

	short := bytes.NewBufferString("ARRAY 5\n1\n2\n")
	dst2 := NewArray[int]()
	assert.ErrorIs(t, LoadTextArray(short, dst2), ErrBadFormat)
}

func TestTextUnsupportedType(t *testing.T) {
	a := NewArray[float64]()
	a.PushBack(1.5)
	var buf bytes.Buffer
	assert.ErrorIs(t, SaveTextArray(&buf, a), ErrBadFormat)

	bad := bytes.NewBufferString("ARRAY 1\n1\n")
	f := NewArray[float64]()
	assert.ErrorIs(t, LoadTextArray(bad, f), ErrBadFormat)
}
