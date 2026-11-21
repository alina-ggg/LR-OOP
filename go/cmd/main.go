package main

import (
	"fmt"
	"log"
	"os"

	ads "lr3go"
)

func ensureDir(path string) {
	if err := os.MkdirAll(path, 0o755); err != nil {
		log.Fatalf("Ошибка создания директории %s: %v", path, err)
	}
}

func main() {
	fmt.Println("  Лабораторная работа: Структуры данных (Go)")
	fmt.Println("  Демонстрация операций и сериализации")

	fmt.Println("1. Базовые операции")

	// 1. Массив
	a := ads.NewArray[int]()
	for i := 1; i <= 5; i++ {
		a.PushBack(i * 10)
	}
	fmt.Printf("Array: size=%d, capacity=%d\n", a.Size(), a.Capacity())

	// 2. Стек
	st := ads.NewStack[int]()
	st.Push(1)
	st.Push(2)
	st.Push(3)
	v, _ := st.Pop()
	fmt.Printf("Stack pop: %d (LIFO)\n", v)

	// 3. Очередь
	q := ads.NewQueue[string]()
	q.Enqueue("first")
	q.Enqueue("second")
	f, _ := q.Front()
	fmt.Printf("Queue front: %s (FIFO)\n", f)

	// 4. Хеш-таблица
	h := ads.NewHashTable[string, int](ads.HashString)
	h.Insert("alice", 20)
	h.Insert("bob", 25)
	h.Insert("alice", 21) // Обновление
	av, _ := h.Get("alice")
	fmt.Printf("HashTable: alice=%d, load factor=%.2f\n", av, h.LoadFactor())

	// 5. Дерево
	tr := ads.NewCompleteBinaryTree[int]()
	for i := 1; i <= 7; i++ {
		tr.Insert(i)
	}
	r, _ := tr.Root()
	fmt.Printf("Tree: root=%d, height=%d\n", r, tr.Height())

	fmt.Println("\n 2. Демонстрация сериализации ")

	outputDir := "data/output"
	ensureDir(outputDir)

	fmt.Println("\n[Array[int]]")

	// Текстовая
	txtArrayPath := outputDir + "/go_array.txt"
	if fh, err := os.Create(txtArrayPath); err == nil {
		ads.SaveTextArray(fh, a)
		fh.Close()
		fmt.Println("  [OK] Сохранено в текстовый файл:", txtArrayPath)
	}

	// Бинарная
	binArrayPath := outputDir + "/go_array.bin"
	if fh, err := os.Create(binArrayPath); err == nil {
		ads.SaveBinaryArray(fh, a)
		fh.Close()
		fmt.Println("  [OK] Сохранено в бинарный файл:", binArrayPath)
	}

	fmt.Println("\n[HashTable[string, int]]")

	// Текстовая
	txtHashPath := outputDir + "/go_hash.txt"
	if fh, err := os.Create(txtHashPath); err == nil {
		ads.SaveTextHashTable(fh, h)
		fh.Close()
		fmt.Println("  [OK] Сохранено в текстовый файл:", txtHashPath)
	}

	// Бинарная
	binHashPath := outputDir + "/go_hash.bin"
	if fh, err := os.Create(binHashPath); err == nil {
		ads.SaveBinaryHashTable(fh, h)
		fh.Close()
		fmt.Println("  [OK] Сохранено в бинарный файл:", binHashPath)
	}

	fmt.Println("\n3. Проверка загрузки ")

	// Загрузка бинарного массива
	loadedBinArray := ads.NewArray[int]()
	if fh, err := os.Open(binArrayPath); err == nil {
		if err := ads.LoadBinaryArray(fh, loadedBinArray); err == nil {
			fmt.Printf("  ✓ Бинарный Array загружен. Размер: %d, 3-й элемент: ", loadedBinArray.Size())
			val, _ := loadedBinArray.At(2)
			fmt.Printf("%d\n", val)
		} else {
			fmt.Printf("  [ERROR] Ошибка загрузки бинарного Array: %v\n", err)
		}
		fh.Close()
	}

	// Загрузка текстовой хеш-таблицы
	loadedTxtHash := ads.NewHashTable[string, int](ads.HashString)
	if fh, err := os.Open(txtHashPath); err == nil {
		if err := ads.LoadTextHashTable(fh, loadedTxtHash); err == nil {
			val, _ := loadedTxtHash.Get("bob")
			fmt.Printf("  ✓ Текстовый HashTable загружен. bob = %d\n", val)
		} else {
			fmt.Printf("  [ERROR] Ошибка загрузки текстового HashTable: %v\n", err)
		}
		fh.Close()
	}

	fmt.Println("  Демонстрация завершена успешно!")
	fmt.Println("  Файлы сохранены в папке:", outputDir)
}
