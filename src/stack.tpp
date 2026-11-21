#ifndef STACK_TPP
#define STACK_TPP

template <typename T>
void Stack<T>::push(const T& value) {
    data_.pushBack(value);
}

template <typename T>
T Stack<T>::pop() {
    if (isEmpty()) {
        throw std::out_of_range("Stack::pop: stack is empty");
    }
    return data_.popBack();
}

template <typename T>
T& Stack<T>::top() {
    if (isEmpty()) {
        throw std::out_of_range("Stack::top: stack is empty");
    }
    return data_.at(data_.getSize() - 1);
}

template <typename T>
const T& Stack<T>::top() const {
    if (isEmpty()) {
        throw std::out_of_range("Stack::top: stack is empty");
    }
    return data_.at(data_.getSize() - 1);
}

template <typename T>
std::size_t Stack<T>::getSize() const {
    return data_.getSize();
}

template <typename T>
bool Stack<T>::isEmpty() const {
    return data_.isEmpty();
}

template <typename T>
void Stack<T>::clear() {
    data_.clear();
}

#endif