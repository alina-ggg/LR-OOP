package main

import (
	"fmt"
	"os"

	ads "lr3go"
)

func main() {
	a := ads.NewArray[int]()
	for i := 1; i <= 5; i++ {
		a.PushBack(i * 10)
	}
	fmt.Println("array size:", a.Size(), "capacity:", a.Capacity())

	st := ads.NewStack[int]()
	st.Push(1)
	st.Push(2)
	v, _ := st.Pop()
	fmt.Println("stack pop:", v)

	q := ads.NewQueue[string]()
	q.Enqueue("first")
	q.Enqueue("second")
	f, _ := q.Front()
	fmt.Println("queue front:", f)

	h := ads.NewHashTable[string, int](ads.HashString)
	h.Insert("alice", 20)
	h.Insert("bob", 25)
	av, _ := h.Get("alice")
	fmt.Println("hash alice:", av, "load factor:", h.LoadFactor())

	tr := ads.NewCompleteBinaryTree[int]()
	for i := 1; i <= 7; i++ {
		tr.Insert(i)
	}
	r, _ := tr.Root()
	fmt.Println("tree root:", r, "height:", tr.Height())

	// текстовая сериализация
	if err := os.MkdirAll("data/output", 0o755); err != nil {
		panic(err)
	}
	if fh, err := os.Create("data/output/go_array.txt"); err == nil {
		_ = ads.SaveTextArray(fh, a)
		fh.Close()
	}
	if fh, err := os.Create("data/output/go_hash.txt"); err == nil {
		_ = ads.SaveTextHashTable(fh, h)
		fh.Close()
	}
	fmt.Println("text serialization written to go/data/output/")
}
