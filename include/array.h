#ifndef ARRAY_H
#define ARRAY_H

#include <cstddef>   // std::size_t
#include <stdexcept> // std::out_of_range

// Шаблонный класс "Динамический массив" (на основе ЛР1)
template <typename T> class Array {
private:
  T *data_;          // указатель на буфер элементов
  std::size_t size_; // количество элементов
  std::size_t capacity_; // вместимость буфера

  // Вспомогательный метод: перевыделение буфера (скрыт от пользователя)
  void resize(std::size_t new_capacity);

public:
  // --- Конструкторы / деструктор ("правило пяти") ---
  Array();                              // по умолчанию
  explicit Array(std::size_t capacity); // с начальной вместимостью
  Array(const Array &other);            // копирования
  Array(Array &&other) noexcept; // перемещения
  ~Array();                      // деструктор

  Array &operator=(const Array &other); // копирующее присваивание
  Array &operator=(Array &&other) noexcept; // перемещающее присваивание

  // --- Базовые операции (как в ЛР1) ---
  void pushBack(const T &value); // добавить в конец
  void insertAt(std::size_t index, const T &value); // вставить по индексу
  void removeAt(std::size_t index); // удалить по индексу
  T popBack(); // удалить и вернуть последний

  // --- Доступ к элементам ---
  T &at(std::size_t index); // с проверкой границ
  const T &at(std::size_t index) const; // const-версия (для const объектов)
  T &operator[](std::size_t index);
  const T &operator[](std::size_t index) const;

  // --- Информационные методы (константные!) ---
  std::size_t getSize() const;
  std::size_t getCapacity() const;
  bool isEmpty() const;
  void clear(); // очистить (вместимость не меняем)
};

// Подключаем реализацию шаблона в конце заголовка
#include "../src/array.tpp"

#endif // ARRAY_H