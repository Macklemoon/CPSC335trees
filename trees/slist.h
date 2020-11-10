//
//  slist.h
//  stacks_n_queues
//
//  Created by William McCarthy on 277//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#ifndef __slist_h__
#define __slist_h__

//#include "iterator.h"

template <typename T>
void print(const std::string& msg, int width, const T& value) {
  std::cout << msg << " " << std::setw(width) << value;
}
//-----------------------------------------------------------
template <typename T>
struct node {
  node() : node(T()) { }
  node(const T& value, node* next=nullptr) : value_(value), next_(next) { }
  friend std::ostream& operator<<(std::ostream& os, const node& no) {
    return os << no.value_ << " ";
  }
  T value_;
  node<T>* next_;
  
  static void run_tests() {
    std::cout << "run_node_tests.......................................\n";
    node<std::string> no1, no2("so do all"), no3("who live in such times", &no2);
    std::cout << "no1: " << no1 << ", no2: " << no2 << ", no3: " << no3 << "\n\n";
  }
};
//-----------------------------------------------------------
template <typename T>
class slist {
public:
  //-----------------------------------------------------------
  class iterator {
  public:
    iterator(slist& list) : iterator(list, list.head_) {}
    iterator(slist& list, node<T>* no)
    : list_(list), current_(no) { }
    iterator(const iterator& other) : iterator(other.list_) { copy(other); }
    iterator& operator=(const iterator& other) {
      if (this != &other) { copy(other); }
      return *this;
    }
    iterator& operator++() {
      check_overflow();
      current_ = current_->next_;
      return *this;
    }
    iterator& operator++(int) {
      check_overflow();
      iterator it = *this;
      current_ = current_->next_;
      return it;
    }
    T& operator*() {
      check_overflow();
      return current_->value_;
    }
    
    bool has_next() { return list_.current_ != nullptr; }       // 0 1 2 3 4  size == 5, current == 3,
    T next() {
      if (!has_next()) { throw new std::overflow_error("no next element in stack\n"); }
      T& value = current_->value_;
      current_ = current_->next_;
      return value;
    }
    
    bool isEqual(const iterator& other) const { return current_ == other.current_; }
    bool operator==(const iterator& other) const { return isEqual(other); }
    bool operator!=(const iterator& other) const { return !operator==(other); }

  private:
    void copy(const iterator& other) { list_ = other.list_;  current_ = other.current_; }
    void check_overflow() {
      if (current_ == nullptr) { throw new std::logic_error("iterator exceeded bounds of list\n"); }
    }

    slist& list_;
    node<T>* current_;
  };
  //-----------------------------------------------------------

  slist(bool pushfront=false) : slist(nullptr, nullptr, 0, pushfront) { }
  slist(node<T>* head, node<T>* tail, size_t size, bool pushfront)
  : head_(head), tail_(tail), size_(size), pushfront_(pushfront) { }
  
  slist(const std::initializer_list<T>& li, bool pushfront=false) : slist(pushfront) {
    for (const T& el : li) { pushfront ? push_front(el) : push_back(el); }
  }
  ~slist() { /*    std::cout << "destroying the slist's nodes...\n"; */  clear(); }
  
  void push_front(const T& value) {
    node<T>* p = new node<T>(value, head_);
    head_ = p;
    if (size_ == 0) { tail_ = head_; }
    ++size_;
  }
  void push_back(const T& value) {
    if (size_ == 0) { push_front(value);  return; }
    node<T>* q = new node<T>(value);
    tail_->next_ = q;
    tail_ = q;
    ++size_;
  }
  void check_pop() { if (size_ == 0) { throw new std::invalid_argument("popping empty slist\n"); } }
  void pop_front() {
    check_pop();
    node<T>* p = head_;
    head_ = head_->next_;
    delete p;
    --size_;
  }
  void pop_back() {
    if (size_ == 1 || size_ == 0) { pop_front();  return; }
    node<T>* p = head_;
    while (p->next_ != tail_) { p = p->next_; }
    p->next_ = nullptr;
    delete tail_;
    tail_ = p->next_;
    --size_;
  }
  void clear() {
    node<T>* p = head_;
    while (p != nullptr) {
      node<T>* q = p;
      delete p;
      p = q->next_;
    }
    head_ = tail_ = nullptr;
    size_ = 0;
  }
  node<T>* head() const { return head_; }
  node<T>* tail() const { return tail_; }
  size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }
  class iterator begin() { return iterator(*this); }
  class iterator end() { return iterator(*this, nullptr); }

  friend std::ostream& operator<<(std::ostream& os, const slist& li) {
    if (li.size_ == 0) { return os << "list is empty\n"; }
    node<T>* p = li.head_;
    while (p != nullptr) { os << *p;  p = p->next_; }
    return os;
  }
  static void run_tests() {
    typedef std::initializer_list<std::string> string_list;
    const string_list& li1 = { "one", "two", "three", "four", "five", "six", "seven" };
    const string_list& li2 = { "once", "more", "into", "the", "breach" };
    const string_list& li3 = { "seven", "for", "the", "dwarf", "lords" };
    slist_test(li1, false);
    slist_test(li2, false);
    slist_test(li3, false);

    slist_test(li1, true);
    slist_test(li2, true);
    slist_test(li3, true);
  }
private:
  static void slist_test(const std::initializer_list<T>& init_li, bool pushfront) {
    std::cout << __FUNCTION__ << ".......................................\n";
    slist<T> li(init_li);   // push back by default
    std::cout << "li is: ...\n" << li << "\n";
    
    std::cout << "\nUsing for each loop...\n";
    for (const std::string& el : li) { std::cout << el << ":: "; }
    
    li.clear();
    std::cout << "\n\nli is now after clearing: ...\n" << li;
    std::cout << "................................. end of slist_test\n\n";
  }
  node<T>* head_;
  node<T>* tail_;
  size_t size_;
  bool pushfront_;
};

#endif /* __slist_h__


*/
