//
//  stack.h
//  stacks_n_queues
//
//  Created by William McCarthy on 277//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#ifndef stack_h
#define stack_h

#include "slist.h"
// #include "iterator.h"

template <typename T>
class stack_ {
public:
  //---------------------------------------------------------
  class iterator {
  private:
    stack_<T>& st_;
    size_t current_;
    typename slist<T>::iterator it_;

  public:
    iterator(stack_<T>& st) : iterator(st, 0) { }
    iterator(stack_<T>& st, size_t current)
    : st_(st), current_(current), it_(st_.li_.begin()) {
      for (size_t i = 0; i < current_; ++i) { ++it_; }
    }
    iterator& operator++()    { iterator& now = *this;     ++it_;  ++current_;  return now; }
    iterator& operator++(int) { iterator& before = *this;  ++it_;  ++current_;  return before; }
    T& operator*()            { return *it_; }
    bool operator==(const iterator& other) { return current_ == other.current_; }
    bool operator!=(const iterator& other) { return !operator==(other); }
  };
  //---------------------------------------------------------

  stack_() = default;
  stack_(const std::initializer_list<T>& li) : stack_() {
    for (const T& el : li) { push(el); }
  }
  ~stack_() { /* std::cout << "destroying the slist inside the stack...\n"; */  clear(); }
  
  void push(const T& value) { li_.push_front(value); }
  T pop() {
    if (li_.size() == 0) { throw new std::invalid_argument("stack overflow\n"); }
    T popped_value = top();
    li_.pop_front();
    return popped_value;
  }
  void clear() { li_.clear(); }
  T top() const { return li_.head()->value_; }
  size_t size() const { return li_.size(); }
  bool empty() const { return li_.empty(); }

  class iterator begin() { return iterator(*this); }
  class iterator end()   { return iterator(*this, size()); }

  friend std::ostream& operator<<(std::ostream& os, const stack_& st) {
    if (st.size() == 0) { return os << "stack is empty\n"; }
    os << st.li_;
    return os;
  }
  static void run_tests() {
    stack_test({ "one", "two", "three", "four", "five", "six", "seven" });
    stack_test({ "nine", "one", "eight", "two", "five" });
    stack_test({"three", "rings", "for", "elven"});
  }
  static void stack_test(const std::initializer_list<std::string>& init_li) {
    std::cout << "\n" << __FUNCTION__ << ".......................................\n";
    stack_<std::string> st(init_li);
    std::cout << "st is: ..." << st;
    
    std::string number = "ninety-nine";
    st.push(number);
    std::cout << "\nst is now after pushing " << number << "...\n" << st;
    std::cout << "\ntop is now: " << st.top() << "\n";
    
    std::cout << "\nSTARTING ITERATION...\n";
    for (const std::string& el : st) { std::cout << "for-each -- el is: " << el << "\n"; }

    std::cout << "\npopping stack until it is empty...\n";
    while (!st.empty()) {
      const std::string& popped_value = st.pop();
      std::cout << "popped value: " << popped_value << ", st: " << st;
      if (!st.empty()) { std::cout << "top: " << st.top() << "\n"; }
    }
    st.clear();
    std::cout << "st is now after clearing: ...\n" << st << "\n";
  }
private:
  slist<T> li_;
};


#endif /* stack_h */
