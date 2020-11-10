//
//  utils.h
//  stacks_n_queues
//
//  Created by William McCarthy on 267//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#ifndef __utils_h__
#define __utils_h__

#include <iostream>
#include <fstream>
#include <cstdlib>


#define ARGC_ERROR  1
#define FILENAME_ERROR  2


class begin_end {
public:
  begin_end() : function_name(__FUNCTION__) {
    std::cout << "......... Beginning of " << function_name
              << ".................................\n";
  }
  ~begin_end() {
    std::cout << "................................. end of "
              << function_name << "......... \n";
  }
private:
  std::string function_name;
};


std::ifstream open_cmdline_file(int argc, const char* argv[], size_t args_desired, const std::string& usage_msg, bool exact_args=true) {
  if (argc < args_desired || (exact_args && argc != args_desired)) {
    std::cerr << usage_msg;
    exit(ARGC_ERROR);
  }
  std::string filename = argv[1];
  std::ifstream ifs(filename);
  if (!ifs.is_open()) {
    std::cerr << "Could not open: '" << filename << "'\n";
    exit(FILENAME_ERROR);
  }
  return ifs;
}


//==========================================================================
// std::unique_ptr and std::shared_ptr wrapper class
//==========================================================================
template <typename Ptr, typename Value>
class wrapper {
public:
  wrapper() : wrapped_(Ptr(nullptr)) { }
//  wrapper(const Ptr& ptr) : wrapped_(ptr) { }
  wrapper(Value value) : wrapped_(Ptr(new Value(value))) { }
                      
  Ptr& operator&() { return wrapped_; }

  const Value& operator*() const { return                    deref_helper(); }
        Value& operator*()       { return const_cast<Value&>(deref_helper()); }

  friend std::ostream& operator<<(std::ostream& os, const wrapper<Ptr, Value>& wr) {
    return os << wr.wrapped_ << "\n";
  }
  
private:
  const Value& deref_helper() const {
    static Value invalid = Value();
    if (wrapped_ == nullptr) {
      std::cout << "warning: nullptr dereferenced ";
      return invalid;
    }
    //throw new std::invalid_argument("nullptr dereferenced\n"); }
    return *wrapped_;
  }

  Ptr wrapped_;
};


//==========================================================================
// printing utilities
//==========================================================================
std::string yes_or_no(bool condition) { return condition ? "YES" : "no"; }

template <typename T>
void print(const std::string& msg, T* arr, size_t low, size_t high) {
  std::cout << msg;
  for (size_t i = low; i < high; ++i) { std::cout << arr[i] << " "; }
}

template <typename T>
void print(const std::string& msg, T* arr, size_t n) { print(msg, arr, 0, n - 1); }


char* strconvert(char* s, int (*fp)(int)) {
  char* p = s;
  while (*p != '\0') { *p = fp(*p);  ++p; }
  return s;
}

char* strstrip(char* s) {
  char* p = s;
  while (*p != '\0') {
    if (isalnum(*p) && !ispunct(*p)) { *s++ = *p++; }
    else { ++p; }
  }
  *s = '\0';
  return s;
}
//==========================================================================
// sorting utilities
//==========================================================================
template <typename T>
struct comparator {
  comparator() { }
  virtual bool operator()(const T& v, const T& w) const = 0;
};

template <typename T>
struct fwd_comparator : public comparator<T> {
  virtual bool operator()(const T& v, const T& w) const override {
    return v < w;
  }
};

template <typename T>
struct rev_comparator : public comparator<T> {
  virtual bool operator()(const T& v, const T& w) const override { return w < v; }
};

template <typename T>
struct null_comparator : public comparator<T> {
  virtual bool operator()(const T& v, const T& w) const override { return false; }
};

template <typename T>
struct comparator_lambda : public comparator<T> {
  comparator_lambda(const std::function<bool(const T&, const T&)>& lambda)
  : lambda_(lambda) { }
  
  virtual bool operator()(const T& v, const T& w) const {
    bool result = lambda_(v, w);
    return result;
  }
  std::function <bool(const T&, const T&)> const lambda_;
};

template <typename T>
bool less(T& v, T& w) { return v < w; }

template <typename T>
bool less(T& v, T& w, const comparator<T>& comp) {
  bool value = comp(v, w);
  return value;
}
//  std::cout << "in less..., v is: " << v << ", w is: " << w << "\n";
//  bool comparison = comp(v, w);
//  return comparison;
//}


template <typename T>
bool compare(T& v, T& w) {
  int result;

  if (less(v, w))        { result = -1;
  } else if (less(w, v)) { result = 1;
  } else                 { result = 0; }

  return result;
}

template <typename T>
int compare(T& v, T& w, const comparator<T>& comp) {
  int result;

  if (less(v, w, comp))        { result = -1;
  } else if (less(w, v, comp)) { result = 1;
  } else                       { result = 0; }

  return result;
}


template <typename T>
void exchange(T* a, size_t i, size_t j) {
  T swap = a[i];
  a[i] = a[j];
  a[j] = swap;
}


template <typename T>
bool is_sorted(T* a, size_t low, size_t high) {
  for (size_t i = low + 1; i < high; ++i) {
    if (less(a[i], a[i - 1])) { return false; }
  }
  return true;
}

template <typename T>
bool is_sorted(T* a, size_t low, size_t high, const comparator<T>& comp) {
  for (size_t i = low + 1; i < high; ++i) {
    if (less(a[i], a[i - 1], comp)) { return false; }
  }
  return true;
}


template <typename T>
bool is_sorted(T* a, size_t size) { return is_sorted(a, 0, size - 1); }

template <typename T>
bool is_sorted(T* a, size_t size, const comparator<T>& comp) {
  return is_sorted(a, 0, size - 1, comp);
}




//==========================================================================


#endif /* __utils_h__ */
