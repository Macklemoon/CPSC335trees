//
//  queue.h
//  stacks_n_queues
//
//  Created by William McCarthy on 277//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#ifndef __queue_h__
#define __queue_h__
#include "slist.h"
typedef std::initializer_list<std::string> string_list;
static const string_list& test_li1 = { "one", "two", "three", "four", "five", "six", "seven", "eight", "one", "nine" };
static const string_list& test_li2 = { "once", "more", "into", "the", "breach" };
static const string_list& test_li3 = { "seven", "for", "the", "dwarf", "lords" };

template <typename T, typename U>
static void queue_test(T& q, const std::string& msg, const std::initializer_list<U>& init_li) {
  std::cout << "\n" << msg << ".......................................\n";
  std::cout << "q is: ..." << q;

  std::string number = "ninety-nine";
  q.enqueue(number);
  std::cout << "\nq is now after enqueing " << number << " ...\n" << q;
  std::cout << "front is now: " << q.front() << "\n";

  std::cout << "\ndequeuing queue until it is empty...\n";
  while (!q.empty()) {
    const U& dequeued_value = q.dequeue();
    std::cout << "dequeued value: " << dequeued_value << ", q: ..." << q;
    if (!q.empty()) {
      std::cout << "front is now: " << q.front() << "\n";
    }
  }
//    q.dequeue();   // this should fail when QUEUE_SIZE < items to be enqueued -- and it does!

  q.clear();
  std::cout << "q is now after clearing: " << q << "\n";
}


template <typename T>
class queue_ {
public:
  virtual void enqueue(const T& value) = 0;
  virtual T dequeue() = 0;
  virtual void clear() = 0;
  virtual T front() = 0;
  virtual T tail() = 0;
  virtual size_t size() const = 0;
  virtual bool empty() const = 0;
};


//---------------------------------------------------------------------------------------------------
template <typename T>
class array_queue : public queue_<T> {
public:
//-----------------------------------------------------------------------------------
  class iterator {
  private:
    array_queue& qu_;
    size_t current_;

  public:
    iterator(array_queue& qu) : iterator(qu, 0) { }
    iterator(array_queue& qu, size_t current) : qu_(qu), current_(current) { }
    iterator& operator++() { ++current_;  return *this; }
    iterator& operator++(int) { iterator& before = *this;  current_++;  return *this; }
    T& operator*() { return qu_.data_[current_]; }
    bool operator==(const iterator& other) { return current_ == other.current_; }
    bool operator!=(const iterator& other) { return !operator==(other); }
  };
  //-----------------------------------------------------------------------------------

  class iterator begin() { return iterator(*this); }
  class iterator end()   { return iterator(*this, size()); }

  array_queue() : array_queue(QUEUE_SIZE) { }
  array_queue(size_t capacity)
  : start_(0), end_(0), sz_(0), capacity_(capacity), data_(new T[capacity]) {
    for (size_t i = 0; i < capacity_; ++i) { data_[i] = T(); }
  }
  array_queue(const array_queue& other) : array_queue() { copy(other); }
  array_queue& operator=(const array_queue& other) {
    if (this != &other) { copy(other); }
    return *this;
  }
  void copy(const array_queue& other) {
    if (data_ != nullptr) { delete[] data_; }
    start_ = other.start_;
    end_ = other.end_;
    sz_ = other.sz_;
    capacity_ = other.capacity_;
    data_ = new T[capacity_];
    for (size_t i = 0; i < capacity_; ++i) { data_[i] = other.data_[i]; }
  }
  array_queue(const std::initializer_list<T>& li) : array_queue() {
    for (const T& el : li) {
      enqueue(el);
    }
  }
  ~array_queue() { // std::cout << "destroying the slist inside the list_queue...\n";
    clear();
  }
  void resize(size_t newcapacity) {
    if (sz_ > newcapacity) { throw new std::logic_error("sz is greater than resized capacity!\n"); }
//    std::cout << "\n\nresizing from " << capacity_ << " to " << newcapacity << "...........\n";

    T* newdata = new T[newcapacity];   // 0 1 2 3 4 5 6   start=4, end=2  capacity = 7
    bool two_parts = end_ <= start_ && sz_ > 0;
    size_t first_part = two_parts ? capacity_ - start_ : end_ - start_;
    size_t second_part = two_parts ? end_ : 0;


    for (size_t i = start_, j = 0; i < start_ + first_part; ++i, ++j) { newdata[j] = data_[i]; }          // copy 4 5 6
    for (size_t i = first_part, j = 0; j < second_part; ++i, ++j) { newdata[i] = data_[j]; }  // copy 0 1

    start_ = 0;
    end_ = first_part + second_part;

    data_ = newdata;
    capacity_ = newcapacity;
  }
  void check_overflow()  {
    if (sz_ == capacity_) { //  throw new std::overflow_error("queue overflowed\n"); }
      resize(2 * capacity_);
    }
    if (data_ == nullptr) { throw new std::logic_error("trying to enqueue a queue with no data available\n"); }
  }
  void check_underflow() {
    if (sz_ < capacity_ / 4 && capacity_ > QUEUE_SIZE) { // throw new std::underflow_error("queue underflowed\n"); }
      resize(capacity_ / 2);
    }
    if (sz_ == 0) { throw new std::logic_error("trying to dequeue from an empty queue\n"); }
  }

  void enqueue(const T& value) {
    check_overflow();
    data_[end_] = value;
    end_ = (end_ + 1) % capacity_;
    ++sz_;
  }
  T dequeue() {
    check_underflow();
    T& dequeued_value = data_[start_];
    start_ = (start_ + 1) % capacity_;
    --sz_;
    return dequeued_value;
  }

  void clear() {
    while (sz_ != 0) {
      dequeue();
    }
  }
  T front() {
    check_underflow();
    return data_[start_];
  }
  T tail() {
    check_underflow();
    return data_[end_];
  }
  size_t size() const { return sz_; }
  bool empty() const { return sz_ == 0; }

  friend std::ostream& operator<<(std::ostream& os, const array_queue& q) {
    if (q.size() == 0) { return os << "queue is empty\n"; }
    size_t i = q.start_;
    while (i != q.end_) {
      os << q.data_[i] << " ";
      i = (i + 1) % q.capacity_;
    }
    return os;
  }

  static void run_tests() {
    run_test(test_li1);
    run_test(test_li2);
    run_test(test_li3);
  }

private:    // helper functions
    static void run_test(const string_list& li) {
      array_queue<std::string> q(li);
      queue_test(q, "array_queue_test", li);
    }

private:
  static const size_t QUEUE_SIZE = 10;
  size_t start_;
  size_t end_;
  size_t sz_;
  size_t capacity_;
  T* data_;
};


//---------------------------------------------------------------------------------------------------
template <typename T>
class list_queue : public queue_<T> {
public:
  list_queue() = default;
  list_queue(const std::initializer_list<T>& li) : list_queue() {
    for (const T& el : li) {
      enqueue(el);
    }
  }
  ~list_queue() { // std::cout << "destroying the slist inside the list_queue...\n";
    clear();
  }

  void enqueue(const T& value) { li_.push_back(value); ++size_; }
  T dequeue() {
    if (size_ == 0) { throw new std::invalid_argument("queue underflow\n"); }
    const T& dequeued_value = front();
    li_.pop_front();
    --size_;
    return dequeued_value;
  }

  void clear() { li_.clear(); size_ = 0; }
  T front() const { return li_.head()->value_; }
  T tail() const { return li_.tail()->value_; }
  size_t size() const { return li_.size(); }
  bool empty() const { return li_.empty(); }

  friend std::ostream& operator<<(std::ostream& os, const list_queue& q) {
    if (q.size() == 0) { return os << "queue is empty\n"; }
    os << q.li_;
    return os;
  }

  static void run_tests() {
    run_test(test_li1);
    run_test(test_li2);
    run_test(test_li3);
  }

private:    // helper functions
  static void run_test(const string_list& li) {
    list_queue<T> q(li);
    queue_test(q, "list_queue_test", li);
  }

private:
  slist<T> li_;
  size_t size_;
};


#endif /* __queue_h__ */
