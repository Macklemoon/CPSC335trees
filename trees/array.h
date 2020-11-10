//
//  array.h
//  stacks_queues_bags
//
//  Created by William McCarthy on 148//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#ifndef __array_h__
#define __array_h__

#include <iostream>
#include <cassert>
#include "utils.h"


//---------------------------------------------------------
template <typename T>
class array_ {
public:
  array_() : array_(MIN_CAPACITY_) { }
  array_(const size_t capacity) :
  size_(0), capacity_(capacity), data_(new T[capacity_]) { }
  array_(const std::initializer_list<T>& li) : array_() {
    for (const T& el : li) {
      push_back(el);
    }
  }
  ~array_() {
    std::cout << "deleting array_'s data_...\n";
    delete[] data_;
  }
  array_(const array_& other) : array_() { copy(other); }
  array_& operator=(const array_& other) {
    if (this != &other) { copy(other); }
    return *this;
  }

  void push_back(T value) {
    check_overflow();
    data_[size_++] = value;
  }

  T pop_back() {
    check_underflow();
    T& value = data_[size_ - 1];
    data_[size_--] = T();
    return value;
  }

  T  operator[](size_t i) const { check_range(i);  return data_[i]; }
  T& operator[](size_t i)       { check_range(i);  return data_[i]; }

  void clear() {
    while (!empty()) { pop_back(); }
  }

  bool empty() const { return size_ == 0; }
  size_t size() const { return size_; }
  
  friend std::ostream& operator<<(std::ostream& os, const array_& arr) {
    if (arr.empty()) { return os << "array_ is empty\n"; }
    for (size_t i = 0; i < arr.size(); ++i) {
      os << arr[i] << " ";
    }
    return os;
  }

  static void run_tests() {
    begin_end be;

    array_<std::string> strings = { "Three", "rings", "for", "the", "Elven", "Kings",
      "under", "the", "starry", "sky.\n", "Seven", "for", "the", "Dwarf", "Lords", "in",
      "their", "halls", "of", "stone.\n", "Nine", "for", "Mortal", "Men", "doomed", "to", "die.\n",
      "One", "for", "the", "Dark", "Lord", "on", "his", "dark", "throne.\n",
      "In", "the", "land", "of", "Mordor", "where", "the", "shadows", "lie.\n",
      "One", "Ring", "to", "rule", "them", "all", "One", "Ring", "to", "find", "them.\n",
      "One", "Ring", "to", "bring", "them", "all", "and", "in", "the", "darkness", "bind", "them.\n",
      "In", "the", "land", "of", "Mordor", "where", "the", "shadows", "lie.\n"
    };

    std::cout << "strings...\n " << strings << "\n\n";
  }

private:
  void resize(size_t capacity) {
    if (size_ > capacity) { throw new std::overflow_error("size_ > new capacity...\n"); }

    std::cout << "resizing from " << capacity_ << " to " << capacity << "\n";
    T* newdata = new T[capacity];
    std::copy(data_, data_ + size_, newdata);
    delete[] data_;
    capacity_ = capacity;
    data_ = newdata;
    std::cout << "data_ is now...\n" << *this << "\n";
  }
  void check_underflow() {
    if (size_ == 0) { throw new std::underflow_error("Underflow error\n"); }
    if (size_ < capacity_ / 4 && size_ >= MIN_CAPACITY_) { resize(capacity_ / 2); }
  }
  void check_overflow() {
    if (size_ >= capacity_) { resize(2 * capacity_); }
  }
  void check_range(size_t i) const {
    if (i >= capacity_)    { throw new std::underflow_error("underflow error"); }
    if (i > size_) { throw new std::overflow_error("overflow error\n"); }
  }
  void copy(const array_& other) {
    if (data_ != nullptr) { delete[] data_; }

    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = new T[capacity_];
    std::copy(data_, data_ + capacity_, other.data_);
  }

  static const size_t MIN_CAPACITY_;
  size_t size_;
  size_t capacity_;
  T* data_;
};

template <typename T>
const size_t array_<T>::MIN_CAPACITY_ = 10;



#endif /* __array_h__ */
