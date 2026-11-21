package ads

import (
	"encoding/binary"
	"errors"
	"io"
)

const (
	MagicNumber uint32 = 0x4C523353 // "LR3S"
	Version     uint32 = 1
)

// getTag определяет тег на основе типа данных
func getTag[T any](prefix string) string {
	var zero T
	switch any(zero).(type) {
	case int:
		return prefix + "_INT"
	case string:
		return prefix + "_STR"
	default:
		return prefix
	}
}

func writeBinaryHeader(w io.Writer, tag string, n int) error {
	if err := binary.Write(w, binary.LittleEndian, MagicNumber); err != nil {
		return err
	}
	if err := binary.Write(w, binary.LittleEndian, Version); err != nil {
		return err
	}
	if err := binary.Write(w, binary.LittleEndian, uint32(len(tag))); err != nil {
		return err
	}
	if _, err := w.Write([]byte(tag)); err != nil {
		return err
	}
	return binary.Write(w, binary.LittleEndian, uint64(n))
}

func readBinaryHeader(r io.Reader, expectedTag string) (int, error) {
	var magic, version, tagLen uint32
	var n uint64

	if err := binary.Read(r, binary.LittleEndian, &magic); err != nil {
		return 0, err
	}
	if magic != MagicNumber {
		return 0, errors.New("bad magic number")
	}
	if err := binary.Read(r, binary.LittleEndian, &version); err != nil {
		return 0, err
	}
	if version != Version {
		return 0, errors.New("bad version")
	}
	if err := binary.Read(r, binary.LittleEndian, &tagLen); err != nil {
		return 0, err
	}
	tagBytes := make([]byte, tagLen)
	if _, err := io.ReadFull(r, tagBytes); err != nil {
		return 0, err
	}
	if string(tagBytes) != expectedTag {
		return 0, errors.New("bad tag")
	}
	if err := binary.Read(r, binary.LittleEndian, &n); err != nil {
		return 0, err
	}
	return int(n), nil
}

func writeBinaryValue[T any](w io.Writer, v T) error {
	switch val := any(v).(type) {
	case int:
		return binary.Write(w, binary.LittleEndian, int32(val))
	case string:
		bytes := []byte(val)
		if err := binary.Write(w, binary.LittleEndian, uint32(len(bytes))); err != nil {
			return err
		}
		_, err := w.Write(bytes)
		return err
	default:
		return errors.New("unsupported type for binary serialization")
	}
}

func readBinaryValue[T any](r io.Reader) (T, error) {
	var zero T
	switch any(zero).(type) {
	case int:
		var val int32
		if err := binary.Read(r, binary.LittleEndian, &val); err != nil {
			return zero, err
		}
		return any(int(val)).(T), nil
	case string:
		var length uint32
		if err := binary.Read(r, binary.LittleEndian, &length); err != nil {
			return zero, err
		}
		bytes := make([]byte, length)
		if _, err := io.ReadFull(r, bytes); err != nil {
			return zero, err
		}
		return any(string(bytes)).(T), nil
	default:
		return zero, errors.New("unsupported type for binary deserialization")
	}
}

func SaveBinaryArray[T any](w io.Writer, a *Array[T]) error {
	if err := writeBinaryHeader(w, getTag[T]("ARRAY"), a.Size()); err != nil {
		return err
	}
	for i := 0; i < a.Size(); i++ {
		v, err := a.At(i)
		if err != nil {
			return err
		}
		if err := writeBinaryValue(w, v); err != nil {
			return err
		}
	}
	return nil
}

func LoadBinaryArray[T any](r io.Reader, a *Array[T]) error {
	n, err := readBinaryHeader(r, getTag[T]("ARRAY"))
	if err != nil {
		return err
	}
	a.Clear()
	for i := 0; i < n; i++ {
		v, err := readBinaryValue[T](r)
		if err != nil {
			return err
		}
		a.PushBack(v)
	}
	return nil
}

func SaveBinarySingly[T any](w io.Writer, l *SinglyLinkedList[T]) error {
	if err := writeBinaryHeader(w, getTag[T]("SINGLY"), l.Size()); err != nil {
		return err
	}
	for i := 0; i < l.Size(); i++ {
		v, _ := l.At(i)
		writeBinaryValue(w, v)
	}
	return nil
}

func LoadBinarySingly[T any](r io.Reader, l *SinglyLinkedList[T]) error {
	n, err := readBinaryHeader(r, getTag[T]("SINGLY"))
	if err != nil {
		return err
	}
	l.Clear()
	for i := 0; i < n; i++ {
		v, _ := readBinaryValue[T](r)
		l.PushBack(v)
	}
	return nil
}

func SaveBinaryDoubly[T any](w io.Writer, l *DoublyLinkedList[T]) error {
	if err := writeBinaryHeader(w, getTag[T]("DOUBLY"), l.Size()); err != nil {
		return err
	}
	for i := 0; i < l.Size(); i++ {
		v, _ := l.At(i)
		writeBinaryValue(w, v)
	}
	return nil
}

func LoadBinaryDoubly[T any](r io.Reader, l *DoublyLinkedList[T]) error {
	n, err := readBinaryHeader(r, getTag[T]("DOUBLY"))
	if err != nil {
		return err
	}
	l.Clear()
	for i := 0; i < n; i++ {
		v, _ := readBinaryValue[T](r)
		l.PushBack(v)
	}
	return nil
}

// сохраняем снизу вверх, чтобы при чтении Push восстановил правильный порядок

func SaveBinaryStack[T any](w io.Writer, s *Stack[T]) error {
	if err := writeBinaryHeader(w, getTag[T]("STACK"), s.Size()); err != nil {
		return err
	}
	tmp := *s
	elems := make([]T, 0, tmp.Size())
	for !tmp.IsEmpty() {
		v, _ := tmp.Pop()
		elems = append(elems, v)
	}
	for i, j := 0, len(elems)-1; i < j; i, j = i+1, j-1 {
		elems[i], elems[j] = elems[j], elems[i]
	}
	for _, v := range elems {
		writeBinaryValue(w, v)
	}
	return nil
}

func LoadBinaryStack[T any](r io.Reader, s *Stack[T]) error {
	n, err := readBinaryHeader(r, getTag[T]("STACK"))
	if err != nil {
		return err
	}
	s.Clear()
	for i := 0; i < n; i++ {
		v, _ := readBinaryValue[T](r)
		s.Push(v)
	}
	return nil
}

func SaveBinaryQueue[T any](w io.Writer, q *Queue[T]) error {
	if err := writeBinaryHeader(w, getTag[T]("QUEUE"), q.Size()); err != nil {
		return err
	}
	tmp := *q
	for !tmp.IsEmpty() {
		v, _ := tmp.Dequeue()
		writeBinaryValue(w, v)
	}
	return nil
}

func LoadBinaryQueue[T any](r io.Reader, q *Queue[T]) error {
	n, err := readBinaryHeader(r, getTag[T]("QUEUE"))
	if err != nil {
		return err
	}
	q.Clear()
	for i := 0; i < n; i++ {
		v, _ := readBinaryValue[T](r)
		q.Enqueue(v)
	}
	return nil
}

func SaveBinaryHashTable[K comparable, V any](w io.Writer, t *HashTable[K, V]) error {
	if err := writeBinaryHeader(w, "HASH", t.Size()); err != nil {
		return err
	}
	for _, chain := range t.buckets {
		for _, e := range chain {
			writeBinaryValue(w, e.key)
			writeBinaryValue(w, e.value)
		}
	}
	return nil
}

func LoadBinaryHashTable[K comparable, V any](r io.Reader, t *HashTable[K, V]) error {
	n, err := readBinaryHeader(r, "HASH")
	if err != nil {
		return err
	}
	t.Clear()
	for i := 0; i < n; i++ {
		k, _ := readBinaryValue[K](r)
		v, _ := readBinaryValue[V](r)
		t.Insert(k, v)
	}
	return nil
}

func SaveBinaryTree[T any](w io.Writer, tr *CompleteBinaryTree[T]) error {
	if err := writeBinaryHeader(w, getTag[T]("TREE"), tr.Size()); err != nil {
		return err
	}
	for i := 0; i < tr.Size(); i++ {
		v, _ := tr.At(i)
		writeBinaryValue(w, v)
	}
	return nil
}

func LoadBinaryTree[T any](r io.Reader, tr *CompleteBinaryTree[T]) error {
	n, err := readBinaryHeader(r, getTag[T]("TREE"))
	if err != nil {
		return err
	}
	tr.Clear()
	for i := 0; i < n; i++ {
		v, _ := readBinaryValue[T](r)
		tr.Insert(v)
	}
	return nil
}
