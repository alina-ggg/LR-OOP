package ads

type entry[K comparable, V any] struct {
	key   K
	value V
}

// HashTable — хеш-таблица методом цепочек; rehash при load factor > 0.75.
type HashTable[K comparable, V any] struct {
	buckets [][]entry[K, V]
	size    int
	hash    func(K) uint64
}

const defaultBuckets = 8
const maxLoadFactor = 0.75

func NewHashTable[K comparable, V any](hash func(K) uint64) *HashTable[K, V] {
	return &HashTable[K, V]{buckets: make([][]entry[K, V], defaultBuckets), hash: hash}
}

// HashInt — мультипликативный хеш (золотое сечение Кнута)
func HashInt(k int) uint64 { return uint64(uint32(k)) * 2654435761 }

func HashString(k string) uint64 {
	h := uint64(14695981039346656037)
	for i := 0; i < len(k); i++ {
		h ^= uint64(k[i])
		h *= 1099511628211
	}
	return h
}

func (t *HashTable[K, V]) Size() int     { return t.size }
func (t *HashTable[K, V]) IsEmpty() bool { return t.size == 0 }

func (t *HashTable[K, V]) BucketCount() int { return len(t.buckets) }

func (t *HashTable[K, V]) LoadFactor() float64 {
	return float64(t.size) / float64(len(t.buckets))
}

func (t *HashTable[K, V]) BucketIndexOf(k K) int {
	return int(t.hash(k) % uint64(len(t.buckets)))
}

func (t *HashTable[K, V]) BucketSize(i int) int { return len(t.buckets[i]) }

func (t *HashTable[K, V]) Insert(k K, v V) {
	idx := t.BucketIndexOf(k)
	for i := range t.buckets[idx] {
		if t.buckets[idx][i].key == k {
			t.buckets[idx][i].value = v // обновление существующего ключа
			return
		}
	}
	t.buckets[idx] = append(t.buckets[idx], entry[K, V]{key: k, value: v})
	t.size++
	if t.LoadFactor() > maxLoadFactor {
		t.rehash()
	}
}

func (t *HashTable[K, V]) Get(k K) (V, error) {
	var zero V
	idx := t.BucketIndexOf(k)
	for i := range t.buckets[idx] {
		if t.buckets[idx][i].key == k {
			return t.buckets[idx][i].value, nil
		}
	}
	return zero, ErrKeyNotFound
}

func (t *HashTable[K, V]) Contains(k K) bool {
	_, err := t.Get(k)
	return err == nil
}

func (t *HashTable[K, V]) Remove(k K) error {
	idx := t.BucketIndexOf(k)
	chain := t.buckets[idx]
	for i := range chain {
		if chain[i].key == k {
			t.buckets[idx] = append(chain[:i], chain[i+1:]...)
			t.size--
			return nil
		}
	}
	return ErrKeyNotFound
}

func (t *HashTable[K, V]) Clear() {
	t.buckets = make([][]entry[K, V], defaultBuckets)
	t.size = 0
}

func (t *HashTable[K, V]) rehash() {
	newBuckets := make([][]entry[K, V], len(t.buckets)*2)
	for _, chain := range t.buckets {
		for _, e := range chain {
			idx := int(t.hash(e.key) % uint64(len(newBuckets)))
			newBuckets[idx] = append(newBuckets[idx], e)
		}
	}
	t.buckets = newBuckets
}
