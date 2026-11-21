package ads

import (
	"bytes"
	"encoding/binary"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestBinaryArrayRoundTrip(t *testing.T) {
	src := NewArray[int]()
	src.PushBack(10)
	src.PushBack(20)
	src.PushBack(30)

	var buf bytes.Buffer
	require.NoError(t, SaveBinaryArray(&buf, src))

	dst := NewArray[int]()
	require.NoError(t, LoadBinaryArray(&buf, dst))

	assert.Equal(t, 3, dst.Size())
	v, _ := dst.At(1)
	assert.Equal(t, 20, v)
}

func TestBinaryStringArrayRoundTrip(t *testing.T) {
	src := NewArray[string]()
	src.PushBack("hello")
	src.PushBack("world\nwith newline")

	var buf bytes.Buffer
	require.NoError(t, SaveBinaryArray(&buf, src))

	dst := NewArray[string]()
	require.NoError(t, LoadBinaryArray(&buf, dst))

	assert.Equal(t, 2, dst.Size())
	v, _ := dst.At(1)
	assert.Equal(t, "world\nwith newline", v)
}

func TestBinaryStackLIFORoundTrip(t *testing.T) {
	src := NewStack[int]()
	src.Push(1)
	src.Push(2)
	src.Push(3)

	var buf bytes.Buffer
	require.NoError(t, SaveBinaryStack(&buf, src))

	dst := NewStack[int]()
	require.NoError(t, LoadBinaryStack(&buf, dst))

	v, _ := dst.Pop()
	assert.Equal(t, 3, v) // LIFO: первым выходит 3
	v, _ = dst.Pop()
	assert.Equal(t, 2, v)
}

func TestBinaryQueueFIFORoundTrip(t *testing.T) {
	src := NewQueue[int]()
	src.Enqueue(10)
	src.Enqueue(20)

	var buf bytes.Buffer
	require.NoError(t, SaveBinaryQueue(&buf, src))

	dst := NewQueue[int]()
	require.NoError(t, LoadBinaryQueue(&buf, dst))

	v, _ := dst.Dequeue()
	assert.Equal(t, 10, v) // FIFO: первым выходит 10
}

func TestBinaryHashTableRoundTrip(t *testing.T) {
	src := NewHashTable[string, int](HashString)
	src.Insert("alice", 20)
	src.Insert("bob", 25)

	var buf bytes.Buffer
	require.NoError(t, SaveBinaryHashTable(&buf, src))

	dst := NewHashTable[string, int](HashString)
	require.NoError(t, LoadBinaryHashTable(&buf, dst))

	assert.Equal(t, 2, dst.Size())
	v, _ := dst.Get("alice")
	assert.Equal(t, 20, v)
}

func TestBinaryBadMagicThrows(t *testing.T) {
	var buf bytes.Buffer
	binary.Write(&buf, binary.LittleEndian, uint32(0xDEADBEEF))

	dst := NewArray[int]()
	err := LoadBinaryArray(&buf, dst)
	assert.Error(t, err)
	assert.Contains(t, err.Error(), "bad magic number")
}

func TestBinaryBadTagThrows(t *testing.T) {
	var buf bytes.Buffer
	binary.Write(&buf, binary.LittleEndian, MagicNumber)
	binary.Write(&buf, binary.LittleEndian, Version)
	binary.Write(&buf, binary.LittleEndian, uint32(5)) // длина тега
	buf.Write([]byte("WRONG_TAG"))                     // неправильный тег
	binary.Write(&buf, binary.LittleEndian, uint64(0)) // размер

	dst := NewArray[int]()
	err := LoadBinaryArray(&buf, dst)
	assert.Error(t, err)
	assert.Contains(t, err.Error(), "bad tag")
}
func TestBinarySinglyRoundTrip(t *testing.T) {
	src := NewSinglyLinkedList[int]()
	src.PushBack(1)
	src.PushBack(2)
	src.PushBack(3)

	var buf bytes.Buffer
	require.NoError(t, SaveBinarySingly(&buf, src))

	dst := NewSinglyLinkedList[int]()
	require.NoError(t, LoadBinarySingly(&buf, dst))

	assert.Equal(t, 3, dst.Size())
	v, err := dst.At(1)
	require.NoError(t, err)
	assert.Equal(t, 2, v)
}

func TestBinaryDoublyRoundTrip(t *testing.T) {
	src := NewDoublyLinkedList[int]()
	src.PushBack(10)
	src.PushBack(20)
	src.PushBack(30)

	var buf bytes.Buffer
	require.NoError(t, SaveBinaryDoubly(&buf, src))

	dst := NewDoublyLinkedList[int]()
	require.NoError(t, LoadBinaryDoubly(&buf, dst))

	assert.Equal(t, 3, dst.Size())
	v, err := dst.At(2)
	require.NoError(t, err)
	assert.Equal(t, 30, v)
}

func TestBinaryTreeRoundTrip(t *testing.T) {
	src := NewCompleteBinaryTree[int]()
	for i := 1; i <= 7; i++ {
		src.Insert(i)
	}

	var buf bytes.Buffer
	require.NoError(t, SaveBinaryTree(&buf, src))

	dst := NewCompleteBinaryTree[int]()
	require.NoError(t, LoadBinaryTree(&buf, dst))

	assert.Equal(t, 7, dst.Size())
	assert.Equal(t, 3, dst.Height())
	r, err := dst.Root()
	require.NoError(t, err)
	assert.Equal(t, 1, r)
}

func TestBinarySinglyEmpty(t *testing.T) {
	src := NewSinglyLinkedList[int]()
	var buf bytes.Buffer
	require.NoError(t, SaveBinarySingly(&buf, src))

	dst := NewSinglyLinkedList[int]()
	require.NoError(t, LoadBinarySingly(&buf, dst))
	assert.Equal(t, 0, dst.Size())
}

func TestBinaryDoublyEmpty(t *testing.T) {
	src := NewDoublyLinkedList[int]()
	var buf bytes.Buffer
	require.NoError(t, SaveBinaryDoubly(&buf, src))

	dst := NewDoublyLinkedList[int]()
	require.NoError(t, LoadBinaryDoubly(&buf, dst))
	assert.Equal(t, 0, dst.Size())
}

func TestBinaryTreeEmpty(t *testing.T) {
	src := NewCompleteBinaryTree[int]()
	var buf bytes.Buffer
	require.NoError(t, SaveBinaryTree(&buf, src))

	dst := NewCompleteBinaryTree[int]()
	require.NoError(t, LoadBinaryTree(&buf, dst))
	assert.Equal(t, 0, dst.Size())
}
