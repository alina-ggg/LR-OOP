#ifndef COMPLETE_BINARY_TREE_TPP
#define COMPLETE_BINARY_TREE_TPP

template <typename T>
std::size_t CompleteBinaryTree<T>::parentIndex(std::size_t i) {
    return (i - 1) / 2;
}

template <typename T>
std::size_t CompleteBinaryTree<T>::leftChildIndex(std::size_t i) {
    return 2 * i + 1;
}

template <typename T>
std::size_t CompleteBinaryTree<T>::rightChildIndex(std::size_t i) {
    return 2 * i + 2;
}

template <typename T>
void CompleteBinaryTree<T>::insert(const T& value) {
    nodes_.pushBack(value);  // полнота сохраняется автоматически
}

template <typename T>
void CompleteBinaryTree<T>::removeLast() {
    if (isEmpty()) {
        throw std::out_of_range("CompleteBinaryTree::removeLast: tree is empty");
    }
    nodes_.popBack();
}

template <typename T>
T& CompleteBinaryTree<T>::root() {
    if (isEmpty())
        throw std::out_of_range("CompleteBinaryTree::root: tree is empty");
    return nodes_.at(0);
}

template <typename T>
const T& CompleteBinaryTree<T>::root() const {
    if (isEmpty())
        throw std::out_of_range("CompleteBinaryTree::root: tree is empty");
    return nodes_.at(0);
}

template <typename T>
T& CompleteBinaryTree<T>::at(std::size_t index) {
    return nodes_.at(index);
}

template <typename T>
const T& CompleteBinaryTree<T>::at(std::size_t index) const {
    return nodes_.at(index);
}

template <typename T>
bool CompleteBinaryTree<T>::hasParent(std::size_t index) const {
    return index < nodes_.getSize() && index > 0;
}

template <typename T>
bool CompleteBinaryTree<T>::hasLeftChild(std::size_t index) const {
    return leftChildIndex(index) < nodes_.getSize();
}

template <typename T>
bool CompleteBinaryTree<T>::hasRightChild(std::size_t index) const {
    return rightChildIndex(index) < nodes_.getSize();
}

template <typename T>
T& CompleteBinaryTree<T>::parentOf(std::size_t index) {
    if (!hasParent(index))
        throw std::out_of_range("CompleteBinaryTree::parentOf: no parent");
    return nodes_.at(parentIndex(index));
}

template <typename T>
const T& CompleteBinaryTree<T>::parentOf(std::size_t index) const {
    if (!hasParent(index))
        throw std::out_of_range("CompleteBinaryTree::parentOf: no parent");
    return nodes_.at(parentIndex(index));
}

template <typename T>
T& CompleteBinaryTree<T>::leftChildOf(std::size_t index) {
    if (!hasLeftChild(index))
        throw std::out_of_range("CompleteBinaryTree::leftChildOf: none");
    return nodes_.at(leftChildIndex(index));
}

template <typename T>
const T& CompleteBinaryTree<T>::leftChildOf(std::size_t index) const {
    if (!hasLeftChild(index))
        throw std::out_of_range("CompleteBinaryTree::leftChildOf: none");
    return nodes_.at(leftChildIndex(index));
}

template <typename T>
T& CompleteBinaryTree<T>::rightChildOf(std::size_t index) {
    if (!hasRightChild(index))
        throw std::out_of_range("CompleteBinaryTree::rightChildOf: none");
    return nodes_.at(rightChildIndex(index));
}

template <typename T>
const T& CompleteBinaryTree<T>::rightChildOf(std::size_t index) const {
    if (!hasRightChild(index))
        throw std::out_of_range("CompleteBinaryTree::rightChildOf: none");
    return nodes_.at(rightChildIndex(index));
}

template <typename T>
bool CompleteBinaryTree<T>::contains(const T& value) const {
    for (std::size_t i = 0; i < nodes_.getSize(); ++i) {
        if (nodes_.at(i) == value)
            return true;
    }
    return false;
}

template <typename T>
std::size_t CompleteBinaryTree<T>::getSize() const {
    return nodes_.getSize();
}

template <typename T>
std::size_t CompleteBinaryTree<T>::getHeight() const {
    // высота = floor(log2(n)) + 1
    std::size_t h = 0;
    for (std::size_t n = nodes_.getSize(); n > 0; n >>= 1)
        ++h;
    return h;
}

template <typename T>
bool CompleteBinaryTree<T>::isEmpty() const {
    return nodes_.isEmpty();
}

template <typename T>
void CompleteBinaryTree<T>::clear() {
    nodes_.clear();
}

template <typename T>
bool CompleteBinaryTree<T>::isComplete() const {
    std::size_t n = nodes_.getSize();
    if (n == 0)
        return true;

    // проверяем что для каждого узла i дети находятся в пределах массива
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t left = 2 * i + 1;
        std::size_t right = 2 * i + 2;
        if (left < n && right >= n) {
            // есть левый ребенок, но нет правого — дальше не должно быть узлов
            for (std::size_t j = i + 1; j < n; ++j) {
                if (2 * j + 1 < n)
                    return false;
            }
            return true;
        }
        if (left >= n && right < n)
            return false;
    }
    return true;
}
#endif