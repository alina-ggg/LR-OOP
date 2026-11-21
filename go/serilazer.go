package ads

import (
	"bufio"
	"fmt"
	"io"
	"strconv"
	"strings"
)

func valueToText(v any) (string, error) {
	switch x := v.(type) {
	case int:
		return strconv.Itoa(x), nil
	case string:
		return strconv.Quote(x), nil
	default:
		return "", ErrBadFormat
	}
}

func valueFromText[T any](line string) (T, error) {
	var zero T
	switch any(zero).(type) {
	case int:
		n, err := strconv.Atoi(strings.TrimSpace(line))
		if err != nil {
			return zero, ErrBadFormat
		}
		return any(n).(T), nil
	case string:
		s, err := strconv.Unquote(strings.TrimSpace(line))
		if err != nil {
			return zero, ErrBadFormat
		}
		return any(s).(T), nil
	default:
		return zero, ErrBadFormat
	}
}

func writeHeader(w io.Writer, tag string, n int) error {
	_, err := fmt.Fprintf(w, "%s %d\n", tag, n)
	return err
}

func readHeader(sc *bufio.Scanner, tag string) (int, error) {
	if !sc.Scan() {
		return 0, ErrBadFormat
	}
	fields := strings.Fields(sc.Text())
	if len(fields) != 2 || fields[0] != tag {
		return 0, ErrBadFormat
	}
	n, err := strconv.Atoi(fields[1])
	if err != nil || n < 0 {
		return 0, ErrBadFormat
	}
	return n, nil
}

func readValues[T any](sc *bufio.Scanner, n int) ([]T, error) {
	out := make([]T, 0, n)
	for i := 0; i < n; i++ {
		if !sc.Scan() {
			return nil, ErrBadFormat
		}
		v, err := valueFromText[T](sc.Text())
		if err != nil {
			return nil, err
		}
		out = append(out, v)
	}
	return out, sc.Err()
}

func writeValues[T any](w io.Writer, vals []T) error {
	for _, v := range vals {
		s, err := valueToText(v)
		if err != nil {
			return err
		}
		if _, err := fmt.Fprintln(w, s); err != nil {
			return err
		}
	}
	return nil
}

func SaveTextArray[T any](w io.Writer, a *Array[T]) error {
	if err := writeHeader(w, "ARRAY", a.Size()); err != nil {
		return err
	}
	vals := make([]T, 0, a.Size())
	for i := 0; i < a.Size(); i++ {
		v, err := a.At(i)
		if err != nil {
			return err
		}
		vals = append(vals, v)
	}
	return writeValues(w, vals)
}

func LoadTextArray[T any](r io.Reader, a *Array[T]) error {
	sc := bufio.NewScanner(r)
	n, err := readHeader(sc, "ARRAY")
	if err != nil {
		return err
	}
	vals, err := readValues[T](sc, n)
	if err != nil {
		return err
	}
	a.Clear()
	for _, v := range vals {
		a.PushBack(v)
	}
	return nil
}

func SaveTextSingly[T any](w io.Writer, l *SinglyLinkedList[T]) error {
	if err := writeHeader(w, "SINGLY", l.Size()); err != nil {
		return err
	}
	vals := make([]T, 0, l.Size())
	for i := 0; i < l.Size(); i++ {
		v, err := l.At(i)
		if err != nil {
			return err
		}
		vals = append(vals, v)
	}
	return writeValues(w, vals)
}

func LoadTextSingly[T any](r io.Reader, l *SinglyLinkedList[T]) error {
	sc := bufio.NewScanner(r)
	n, err := readHeader(sc, "SINGLY")
	if err != nil {
		return err
	}
	vals, err := readValues[T](sc, n)
	if err != nil {
		return err
	}
	l.Clear()
	for _, v := range vals {
		l.PushBack(v)
	}
	return nil
}

func SaveTextDoubly[T any](w io.Writer, l *DoublyLinkedList[T]) error {
	if err := writeHeader(w, "DOUBLY", l.Size()); err != nil {
		return err
	}
	vals := make([]T, 0, l.Size())
	for i := 0; i < l.Size(); i++ {
		v, err := l.At(i)
		if err != nil {
			return err
		}
		vals = append(vals, v)
	}
	return writeValues(w, vals)
}

func LoadTextDoubly[T any](r io.Reader, l *DoublyLinkedList[T]) error {
	sc := bufio.NewScanner(r)
	n, err := readHeader(sc, "DOUBLY")
	if err != nil {
		return err
	}
	vals, err := readValues[T](sc, n)
	if err != nil {
		return err
	}
	l.Clear()
	for _, v := range vals {
		l.PushBack(v)
	}
	return nil
}

func SaveTextStack[T any](w io.Writer, s *Stack[T]) error {
	if err := writeHeader(w, "STACK", s.Size()); err != nil {
		return err
	}
	tmp := *s
	vals := make([]T, 0, tmp.Size())
	for !tmp.IsEmpty() {
		v, err := tmp.Pop()
		if err != nil {
			return err
		}
		vals = append(vals, v)
	}
	for i, j := 0, len(vals)-1; i < j; i, j = i+1, j-1 {
		vals[i], vals[j] = vals[j], vals[i] // пишем снизу вверх
	}
	return writeValues(w, vals)
}

func LoadTextStack[T any](r io.Reader, s *Stack[T]) error {
	sc := bufio.NewScanner(r)
	n, err := readHeader(sc, "STACK")
	if err != nil {
		return err
	}
	vals, err := readValues[T](sc, n)
	if err != nil {
		return err
	}
	s.Clear()
	for _, v := range vals {
		s.Push(v)
	}
	return nil
}

func SaveTextQueue[T any](w io.Writer, q *Queue[T]) error {
	if err := writeHeader(w, "QUEUE", q.Size()); err != nil {
		return err
	}
	tmp := *q
	vals := make([]T, 0, tmp.Size())
	for !tmp.IsEmpty() {
		v, err := tmp.Dequeue()
		if err != nil {
			return err
		}
		vals = append(vals, v)
	}
	return writeValues(w, vals)
}

func LoadTextQueue[T any](r io.Reader, q *Queue[T]) error {
	sc := bufio.NewScanner(r)
	n, err := readHeader(sc, "QUEUE")
	if err != nil {
		return err
	}
	vals, err := readValues[T](sc, n)
	if err != nil {
		return err
	}
	q.Clear()
	for _, v := range vals {
		q.Enqueue(v)
	}
	return nil
}

func SaveTextHashTable[K comparable, V any](w io.Writer, t *HashTable[K, V]) error {
	if err := writeHeader(w, "HASH", t.Size()); err != nil {
		return err
	}
	for _, chain := range t.buckets {
		for _, e := range chain {
			ks, err := valueToText(e.key)
			if err != nil {
				return err
			}
			vs, err := valueToText(e.value)
			if err != nil {
				return err
			}
			if _, err := fmt.Fprintf(w, "%s\t%s\n", ks, vs); err != nil {
				return err
			}
		}
	}
	return nil
}

func LoadTextHashTable[K comparable, V any](r io.Reader, t *HashTable[K, V]) error {
	sc := bufio.NewScanner(r)
	n, err := readHeader(sc, "HASH")
	if err != nil {
		return err
	}
	t.Clear()
	for i := 0; i < n; i++ {
		if !sc.Scan() {
			return ErrBadFormat
		}
		parts := strings.SplitN(sc.Text(), "\t", 2)
		if len(parts) != 2 {
			return ErrBadFormat
		}
		k, err := valueFromText[K](parts[0])
		if err != nil {
			return err
		}
		v, err := valueFromText[V](parts[1])
		if err != nil {
			return err
		}
		t.Insert(k, v)
	}
	return sc.Err()
}

func SaveTextTree[T any](w io.Writer, tr *CompleteBinaryTree[T]) error {
	if err := writeHeader(w, "TREE", tr.Size()); err != nil {
		return err
	}
	vals := make([]T, 0, tr.Size())
	for i := 0; i < tr.Size(); i++ {
		v, err := tr.At(i)
		if err != nil {
			return err
		}
		vals = append(vals, v)
	}
	return writeValues(w, vals)
}

func LoadTextTree[T any](r io.Reader, tr *CompleteBinaryTree[T]) error {
	sc := bufio.NewScanner(r)
	n, err := readHeader(sc, "TREE")
	if err != nil {
		return err
	}
	vals, err := readValues[T](sc, n)
	if err != nil {
		return err
	}
	tr.Clear()
	for _, v := range vals {
		tr.Insert(v)
	}
	return nil
}
